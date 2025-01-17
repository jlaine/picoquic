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


uint64_t picoquic_cc_get_sequence_number(picoquic_cnx_t* cnx)
{
    return cnx->pkt_ctx[picoquic_packet_context_application].send_sequence;
}

uint64_t picoquic_cc_get_ack_number(picoquic_cnx_t* cnx)
{
    return cnx->pkt_ctx[picoquic_packet_context_application].highest_acknowledged;
}

void picoquic_filter_rtt_min_max(picoquic_min_max_rtt_t * rtt_track, uint64_t rtt)
{
    int x = rtt_track->sample_current;
    int x_max;


    rtt_track->samples[x] = rtt;

    rtt_track->sample_current = x + 1;
    if (rtt_track->sample_current >= PICOQUIC_MIN_MAX_RTT_SCOPE) {
        rtt_track->is_init = 1;
        rtt_track->sample_current = 0;
    }
    
    x_max = (rtt_track->is_init) ? PICOQUIC_MIN_MAX_RTT_SCOPE : x + 1;

    rtt_track->sample_min = rtt_track->samples[0];
    rtt_track->sample_max = rtt_track->samples[0];

    for (int i = 1; i < x_max; i++) {
        if (rtt_track->samples[i] < rtt_track->sample_min) {
            rtt_track->sample_min = rtt_track->samples[i];
        } else if (rtt_track->samples[i] > rtt_track->sample_max) {
            rtt_track->sample_max = rtt_track->samples[i];
        }
    }
}

int picoquic_hystart_test(picoquic_min_max_rtt_t* rtt_track, uint64_t rtt_measurement, uint64_t current_time)
{
    int ret = 0;

    if(current_time > rtt_track->last_rtt_sample_time + 1000) {
        picoquic_filter_rtt_min_max(rtt_track, rtt_measurement);
        rtt_track->last_rtt_sample_time = current_time;

        if (rtt_track->is_init) {
            uint64_t delta_rtt = 0;

            if (rtt_track->rtt_filtered_min == 0 ||
                rtt_track->rtt_filtered_min > rtt_track->sample_max) {
                rtt_track->rtt_filtered_min = rtt_track->sample_max;
            }

            if (rtt_track->sample_min > rtt_track->rtt_filtered_min) {
                delta_rtt = rtt_track->sample_min - rtt_track->rtt_filtered_min;
                if (delta_rtt * 4 > rtt_track->rtt_filtered_min) {
                    rtt_track->nb_rtt_excess++;
                    if (rtt_track->nb_rtt_excess >= PICOQUIC_MIN_MAX_RTT_SCOPE) {
                        /* RTT increased too much, get out of slow start! */
                        ret = 1;
                    }
                }
                else {
                    rtt_track->nb_rtt_excess = 0;
                }
            }
        }
    }

    return ret;
}