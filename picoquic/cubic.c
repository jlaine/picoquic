/*
* Author: Christian Huitema
* Copyright (c) 2019, Private Octopus, Inc.
* All rights reserved.
*
* Permission to use, copy, modify, and distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Private Octopus, Inc. BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "picoquic_internal.h"
#include <stdlib.h>
#include <string.h>
#include "cc_common.h"

typedef enum {
    picoquic_cubic_alg_slow_start = 0,
    picoquic_cubic_alg_recovery,
    picoquic_cubic_alg_congestion_avoidance
} picoquic_cubic_alg_state_t;

typedef struct st_picoquic_cubic_state_t {
    picoquic_cubic_alg_state_t alg_state;
    uint64_t recovery_sequence;
    uint64_t start_of_epoch;
    uint64_t previous_start_of_epoch;
    double K;
    double W_max;
    double W_last_max;
    double C;
    double beta;
    double W_reno;
    uint64_t ssthresh;
    picoquic_min_max_rtt_t rtt_filter;
} picoquic_cubic_state_t;

void picoquic_cubic_init(picoquic_path_t* path_x)
{
    /* Initialize the state of the congestion control algorithm */
    picoquic_cubic_state_t* cubic_state = (picoquic_cubic_state_t*)malloc(sizeof(picoquic_cubic_state_t));
    path_x->congestion_alg_state = (void*)cubic_state;
    if (cubic_state != NULL) {
        memset(&cubic_state->rtt_filter, 0, sizeof(picoquic_min_max_rtt_t));
        if (path_x->congestion_alg_state != NULL) {
            memset(cubic_state, 0, sizeof(picoquic_cubic_state_t));
            cubic_state->alg_state = picoquic_cubic_alg_slow_start;
            cubic_state->ssthresh = (uint64_t)((int64_t)-1);
            cubic_state->W_last_max = (double)cubic_state->ssthresh / (double)path_x->send_mtu;
            cubic_state->W_max = cubic_state->W_last_max;
            cubic_state->C = 0.4;
            cubic_state->beta = 7.0 / 8.0;
            cubic_state->start_of_epoch = 0;
            cubic_state->previous_start_of_epoch = 0;
            cubic_state->W_reno = PICOQUIC_CWIN_INITIAL;
            cubic_state->recovery_sequence = 0;
            path_x->cwin = PICOQUIC_CWIN_INITIAL;
        }
    }
}

double picoquic_cubic_root(double x)
{
    /* First find an approximation */
    double v = 1;
    double y = 1.0;
    double y2;
    double y3;

    while (v > x * 8) {
        v /= 8;
        y /= 2;
    }

    while (v < x) {
        v *= 8;
        y *= 2;
    }

    for (int i = 0; i < 3; i++) {
        y2 = y * y;
        y3 = y2 * y;
        y += (x - y3) / (3.0*y2);
    }

    return y;
}

/* Compute W_cubic(t) = C * (t - K) ^ 3 + W_max */
static double picoquic_cubic_W_cubic(
    picoquic_cubic_state_t* cubic_state,
    uint64_t current_time)
{
    double delta_t_sec = ((double)(current_time - cubic_state->start_of_epoch) / 1000000.0) - cubic_state->K;
    double W_cubic = (cubic_state->C * (delta_t_sec * delta_t_sec * delta_t_sec)) + cubic_state->W_max;

    return W_cubic;
}

/* On entering congestion avoidance, need to compute the new coefficients
 * of the cubit curve */
static void picoquic_cubic_enter_avoidance(
    picoquic_cubic_state_t* cubic_state,
    uint64_t current_time)
{
    cubic_state->K = picoquic_cubic_root(cubic_state->W_max*(1.0 - cubic_state->beta) / cubic_state->C);
    cubic_state->alg_state = picoquic_cubic_alg_congestion_avoidance;
    cubic_state->start_of_epoch = current_time;
    cubic_state->previous_start_of_epoch = cubic_state->start_of_epoch;
}

/* The recovery state last 1 RTT, during which parameters will be frozen
 */
static void picoquic_cubic_enter_recovery(picoquic_cnx_t * cnx,
    picoquic_path_t* path_x,
    picoquic_congestion_notification_t notification,
    picoquic_cubic_state_t* cubic_state,
    uint64_t current_time)
{
    cubic_state->recovery_sequence = picoquic_cc_get_sequence_number(cnx);
    /* Update similar to new reno, but different beta */
    cubic_state->W_max = (double)path_x->cwin / (double)path_x->send_mtu;
    /* Apply fast convergence */
    if (cubic_state->W_max < cubic_state->W_last_max) {
        cubic_state->W_last_max = cubic_state->W_max;
        cubic_state->W_max = cubic_state->W_max *cubic_state->beta; 
    }
    else {
        cubic_state->W_last_max = cubic_state->W_max;
    }
    /* Compute the new ssthresh */
    cubic_state->ssthresh = (uint64_t)(cubic_state->W_max * cubic_state->beta*(double)path_x->send_mtu);
    if (cubic_state->ssthresh < PICOQUIC_CWIN_MINIMUM) {
        /* If things are that bad, fall back to slow start */
        cubic_state->ssthresh = PICOQUIC_CWIN_MINIMUM;
        cubic_state->alg_state = picoquic_cubic_alg_slow_start;
        cubic_state->ssthresh = (uint64_t)((int64_t)-1);
        cubic_state->previous_start_of_epoch = cubic_state->start_of_epoch;
        cubic_state->start_of_epoch = current_time;
        cubic_state->W_reno = PICOQUIC_CWIN_MINIMUM;
        path_x->cwin = PICOQUIC_CWIN_MINIMUM;
    }
    else {

        if (notification == picoquic_congestion_notification_timeout) {
            path_x->cwin = PICOQUIC_CWIN_MINIMUM;
            cubic_state->previous_start_of_epoch = cubic_state->start_of_epoch;
            cubic_state->start_of_epoch = current_time;
            cubic_state->alg_state = picoquic_cubic_alg_slow_start;
        }
        else {
            /* Enter congestion avoidance immediately */
            picoquic_cubic_enter_avoidance(cubic_state, current_time);
            /* Compute the inital window for both Reno and Cubic */
            double W_cubic = picoquic_cubic_W_cubic(cubic_state, current_time);
            uint64_t win_cubic = (uint64_t)(W_cubic * (double)path_x->send_mtu);
            cubic_state->W_reno = ((double)path_x->cwin) / 2.0;

            /* Pick the largest */
            if (win_cubic > cubic_state->W_reno) {
                /* if cubic is larger than threshold, switch to cubic mode */
                path_x->cwin = win_cubic;
            }
            else {
                path_x->cwin = (uint64_t)cubic_state->W_reno;
            }
        }
    }
}

/* On spurious repeat notification, restore the previous congestion control.
 * Assume that K is still valid -- we only update it after exiting recovery.
 * Set cwin to the value of W_max before the recovery event
 * Set W_max to W_max_last, i.e. the value before the recovery event
 * Set the epoch back to where it was, by computing the inverse of the
 * W_cubic formula */
static void picoquic_cubic_correct_spurious(picoquic_path_t* path_x,
    picoquic_cubic_state_t* cubic_state,
    uint64_t current_time)
{
    cubic_state->W_max = cubic_state->W_last_max;
    picoquic_cubic_enter_avoidance(cubic_state, cubic_state->previous_start_of_epoch);
    double W_cubic = picoquic_cubic_W_cubic(cubic_state, current_time);
    cubic_state->W_reno = W_cubic * (double)path_x->send_mtu;
    cubic_state->ssthresh = (uint64_t)(cubic_state->W_max * cubic_state->beta * (double)path_x->send_mtu);
    path_x->cwin = (uint64_t)cubic_state->W_reno;
}

/*
 * Properly implementing Cubic requires managing a number of
 * signals, such as packet losses or acknowledgements. We attempt
 * to condensate all that in a single API, which could be shared
 * by many different congestion control algorithms.
 */
void picoquic_cubic_notify(
    picoquic_cnx_t* cnx, picoquic_path_t* path_x,
    picoquic_congestion_notification_t notification,
    uint64_t rtt_measurement,
    uint64_t nb_bytes_acknowledged,
    uint64_t lost_packet_number,
    uint64_t current_time)
{
#ifdef _WINDOWS
    UNREFERENCED_PARAMETER(rtt_measurement);
    UNREFERENCED_PARAMETER(lost_packet_number);
#endif
    picoquic_cubic_state_t* cubic_state = (picoquic_cubic_state_t*)path_x->congestion_alg_state;

    if (cubic_state != NULL) {
        switch (cubic_state->alg_state) {
        case picoquic_cubic_alg_slow_start:
            switch (notification) {
            case picoquic_congestion_notification_acknowledgement:
                if (path_x->smoothed_rtt <= PICOQUIC_TARGET_RENO_RTT) {
                    path_x->cwin += nb_bytes_acknowledged;
                }
                else {
                    double delta = ((double)path_x->smoothed_rtt) / ((double)PICOQUIC_TARGET_RENO_RTT);
                    delta *= (double)nb_bytes_acknowledged;
                    path_x->cwin += (uint64_t)delta;
                }
                /* if cnx->cwin exceeds SSTHRESH, exit and go to CA */
                if (path_x->cwin >= cubic_state->ssthresh) {
                    cubic_state->W_reno = ((double)path_x->cwin)/2.0;
                    picoquic_cubic_enter_avoidance(cubic_state, current_time);
                }
                break;
            case picoquic_congestion_notification_ecn_ec:
            case picoquic_congestion_notification_repeat:
            case picoquic_congestion_notification_timeout:
                /* enter recovery */
                if (current_time - cubic_state->start_of_epoch > path_x->smoothed_rtt ||
                    cubic_state->recovery_sequence <= picoquic_cc_get_ack_number(cnx)) {
                    picoquic_cubic_enter_recovery(cnx, path_x, notification, cubic_state, current_time);
                }
                break;
            case picoquic_congestion_notification_spurious_repeat:
                /* Reset CWIN based on ssthresh, not based on current value. */
                picoquic_cubic_correct_spurious(path_x, cubic_state, current_time);
                break;
            case picoquic_congestion_notification_rtt_measurement:
                /* Using RTT increases as signal to get out of initial slow start */
                if (cubic_state->ssthresh == (uint64_t)((int64_t)-1) && picoquic_hystart_test(&cubic_state->rtt_filter, rtt_measurement, current_time)) {
                    /* RTT increased too much, get out of slow start! */
                    cubic_state->ssthresh = path_x->cwin;
                    cubic_state->W_max = (double)path_x->cwin / (double)path_x->send_mtu;
                    cubic_state->W_last_max = cubic_state->W_max;
                    cubic_state->W_reno = ((double)path_x->cwin);
                    picoquic_cubic_enter_avoidance(cubic_state, current_time);
                    /* apply a correction to enter the test phase immediately */
                    uint64_t K_micro = (uint64_t)(cubic_state->K * 1000000.0);
                    if (K_micro > current_time) {
                        cubic_state->K = ((double)current_time) / 1000000.0;
                        cubic_state->start_of_epoch = 0;
                    }
                    else {
                        cubic_state->start_of_epoch = current_time - K_micro;
                    }
                }

                break;
            default:
                /* ignore */
                break;
            }
            break;
        case picoquic_cubic_alg_recovery:
            /* If the notification is coming less than 1RTT after start,
			 * ignore it, unless it is a spurious retransmit detection */
            if (notification == picoquic_congestion_notification_spurious_repeat) {
                /* if spurious repeat of initial loss detected,
                 * exit recovery and reset threshold to pre-entry cwin.
                 */
                picoquic_cubic_correct_spurious(path_x, cubic_state, current_time);
            } else{
                switch (notification) {
                case picoquic_congestion_notification_acknowledgement:
                    /* exit recovery, move to CA or SS, depending on CWIN */
                    cubic_state->alg_state = picoquic_cubic_alg_slow_start;
                    path_x->cwin += nb_bytes_acknowledged;
                    /* if cnx->cwin exceeds SSTHRESH, exit and go to CA */
                    if (path_x->cwin >= cubic_state->ssthresh) {
                        cubic_state->alg_state = picoquic_cubic_alg_congestion_avoidance;
                    }
                    break;
                case picoquic_congestion_notification_spurious_repeat:
                    picoquic_cubic_correct_spurious(path_x, cubic_state, current_time);
                    break;
                case picoquic_congestion_notification_ecn_ec:
                case picoquic_congestion_notification_repeat:
                case picoquic_congestion_notification_timeout:
                    if (current_time - cubic_state->start_of_epoch > path_x->smoothed_rtt ||
                        cubic_state->recovery_sequence <= picoquic_cc_get_ack_number(cnx)) {
                        /* re-enter recovery if this is a new loss */
                        picoquic_cubic_enter_recovery(cnx, path_x, notification, cubic_state, current_time);
                    }
                    break;
                case picoquic_congestion_notification_rtt_measurement:
                default:
                    /* ignore */
                    break;
                }
            } 
            break;

        case picoquic_cubic_alg_congestion_avoidance:
            switch (notification) {
            case picoquic_congestion_notification_acknowledgement: {
                /* Compute the cubic formula */
                double W_cubic = picoquic_cubic_W_cubic(cubic_state, current_time);
                uint64_t win_cubic = (uint64_t)(W_cubic * (double)path_x->send_mtu);
                /* Also compute the Reno formula */
                cubic_state->W_reno += ((double)nb_bytes_acknowledged)*((double)path_x->send_mtu) / cubic_state->W_reno;

                /* Pick the largest */
                if (win_cubic > cubic_state->W_reno) {
                    /* if cubic is larger than threshold, switch to cubic mode */
                    path_x->cwin = win_cubic;
                }
                else {
                    path_x->cwin = (uint64_t)cubic_state->W_reno;
                }
                break;
            }
            case picoquic_congestion_notification_ecn_ec:
            case picoquic_congestion_notification_repeat:
            case picoquic_congestion_notification_timeout:
                if (current_time - cubic_state->start_of_epoch > path_x->smoothed_rtt ||
                    cubic_state->recovery_sequence <= picoquic_cc_get_ack_number(cnx)) {
                    /* re-enter recovery */
                    picoquic_cubic_enter_recovery(cnx, path_x, notification, cubic_state, current_time);
                }
                break;
            case picoquic_congestion_notification_spurious_repeat:
                picoquic_cubic_correct_spurious(path_x, cubic_state, current_time);
                break;
            case picoquic_congestion_notification_rtt_measurement:
            default:
                /* ignore */
                break;
            }
            break;
        default:
            break;
        }

        /* Compute pacing data */
        picoquic_update_pacing_data(path_x);
    }
}

/* Release the state of the congestion control algorithm */
void picoquic_cubic_delete(picoquic_path_t* path_x)
{
    if (path_x->congestion_alg_state != NULL) {
        free(path_x->congestion_alg_state);
        path_x->congestion_alg_state = NULL;
    }
}

/* Definition record for the Cubic algorithm */

#define picoquic_cubic_ID 0x43424942 /* CBIC */

picoquic_congestion_algorithm_t picoquic_cubic_algorithm_struct = {
    picoquic_cubic_ID,
    picoquic_cubic_init,
    picoquic_cubic_notify,
    picoquic_cubic_delete
};

picoquic_congestion_algorithm_t* picoquic_cubic_algorithm = &picoquic_cubic_algorithm_struct;
