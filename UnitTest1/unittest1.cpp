/*
* Author: Christian Huitema
* Copyright (c) 2017, Private Octopus, Inc.
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

#include "stdafx.h"
#include "CppUnitTest.h"
#include "picoquictest/picoquictest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
        TEST_CLASS_INITIALIZE(setup) {
            // avoid large debug spew that slows down the console.
            debug_printf_suspend();
        }

	    TEST_METHOD(test_picohash)
	    {
            int ret = picohash_test();

            Assert::AreEqual(ret, 0);
	    }

        TEST_METHOD(bytestream)
        {
            int ret = bytestream_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(connection_id_print)
        {
            int ret = util_connection_id_print_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(connection_id_parse)
        {
            int ret = util_connection_id_parse_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(sprintf)
        {
            int ret = util_sprintf_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(memcmp)
        {
            int ret = util_memcmp_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(random_tester)
        {
            int ret = random_tester_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(random_gauss)
        {
            int ret = random_gauss_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(random_public_tester)
        {
            int ret = random_public_tester_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(splay)
        {
            int ret = splay_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_cnxcreation)
        {
            int ret = cnxcreation_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_parse_header)
        {
            int ret = parseheadertest();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_pn2pn64)
        {
            int ret = pn2pn64test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_intformat)
        {
            int ret = intformattest();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_varints)
        {
            int ret = varint_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_sack)
        {
            int ret = sacktest();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_skip_frames)
        {
            int ret = skip_frame_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_parse_frames)
        {
            int ret = parse_frame_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_logger)
        {
            int ret = logger_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(test_binlog)
        {
            int ret = binlog_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_TlsStreamFrame)
        {
            int ret = TlsStreamFrameTest();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_StreamZeroFrame)
        {
            int ret = StreamZeroFrameTest();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(stream_splay)
        {
            int ret = stream_splay_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(stream_output)
        {
            int ret = stream_output_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(split_stream_frame)
        {
            int ret = split_stream_frame_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(copy_for_retransmit)
        {
            int ret = test_copy_for_retransmit();

            Assert::AreEqual(ret, 0);
        }

		TEST_METHOD(test_sendack)
		{
			int ret = sendacktest();

			Assert::AreEqual(ret, 0);
		}

        TEST_METHOD(test_ackrange)
        {
            int ret = ackrange_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_ack_of_ack)
        {
            int ret = ack_of_ack_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_sim_link)
        {
            int ret = sim_link_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(cleartext_pn_enc)
        {
            int ret = cleartext_pn_enc_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(cid_global_encrypt)
        {
            int ret = cid_global_encrypt_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(cid_mask_encrypt)
        {
            int ret = cid_mask_encrypt_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(test_pn_enc_1rtt)
        {
            int ret = pn_enc_1rtt_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_cnxid_stash)
        {
            int ret = cnxid_stash_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_new_cnxid)
        {
            int ret = new_cnxid_test();

            Assert::AreEqual(ret, 0);
        }

		TEST_METHOD(test_tls_api)
		{
			int ret = tls_api_test();

			Assert::AreEqual(ret, 0);
		}

        TEST_METHOD(null_sni)
        {
            int ret = null_sni_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(test_silence)
        {
            int ret = tls_api_silence_test();

            Assert::AreEqual(ret, 0);
        }

		TEST_METHOD(test_tls_api_first_loss)
		{
			int ret = tls_api_loss_test(1ull);

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_tls_api_second_loss)
		{
			int ret = tls_api_loss_test(2ull);

			Assert::AreEqual(ret, 0);
		}

        TEST_METHOD(test_server_first_loss)
        {
            int ret = tls_api_server_first_loss_test();

            Assert::AreEqual(ret, 0);
        }

		TEST_METHOD(test_tls_api_client_losses)
		{
			int ret = tls_api_client_losses_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_tls_api_server_losses)
		{
			int ret = tls_api_server_losses_test();

			Assert::AreEqual(ret, 0);
		}

        TEST_METHOD(ddos_amplification)
        {
            int ret = ddos_amplification_test();

            Assert::AreEqual(ret, 0);
        }

		TEST_METHOD(test_tls_api_version_negotiation)
		{
			int ret = tls_api_version_negotiation_test();

			Assert::AreEqual(ret, 0);
		}

        TEST_METHOD(test_transport_param_stream_id)
        {
            int ret = transport_param_stream_id_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(stream_rank)
        {
            int ret = stream_rank_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(stream_id_to_rank)
        {
            int ret = stream_id_to_rank_test();

            Assert::AreEqual(ret, 0);
        }

		TEST_METHOD(test_transport_param)
		{
			int ret = transport_param_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_tls_api_sni)
		{
			int ret = tls_api_sni_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_tls_api_alpn)
		{
			int ret = tls_api_alpn_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_tls_api_wrong_alpn)
		{
			int ret = tls_api_wrong_alpn_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_one_way_stream)
		{
			int ret = tls_api_oneway_stream_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_q_and_r_stream)
		{
			int ret = tls_api_q_and_r_stream_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_q2_and_r2_stream)
		{
			int ret = tls_api_q2_and_r2_stream_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_server_reset)
		{
			int ret = tls_api_server_reset_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_bad_server_reset)
		{
			int ret = tls_api_bad_server_reset_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_very_long_stream)
		{
			int ret = tls_api_very_long_stream_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_very_long_max)
		{
			int ret = tls_api_very_long_max_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_very_long_with_err)
		{
			int ret = tls_api_very_long_with_err_test();

			Assert::AreEqual(ret, 0);
		}

		TEST_METHOD(test_very_long_congestion)
		{
			int ret = tls_api_very_long_congestion_test();

			Assert::AreEqual(ret, 0);
		}

        TEST_METHOD(test_http0dot9)
        {
            int ret = http0dot9_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_retry)
        {
            int ret = tls_api_retry_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_retry_token)
        {
            int ret = tls_retry_token_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(test_two_connections)
        {
            int ret = tls_api_two_connections_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_cleartext_aead)
        {
            int ret = cleartext_aead_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_multiple_versions)
        {
            int ret = tls_api_multiple_versions_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_keep_alive)
        {
          int ret = keep_alive_test();

          Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_sockets)
        {
            int ret = socket_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_sockets_ecn)
        {
            int ret = socket_ecn_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(ticket_store)
        {
            int ret = ticket_store_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(token_store)
        {
            int ret = token_store_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_session_resume)
        {
            int ret = session_resume_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(zero_rtt)
        {
            int ret = zero_rtt_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(zero_rtt_loss)
        {
            int ret = zero_rtt_loss_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_stop_sending)
        {
            int ret = stop_sending_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_unidir)
        {
            int ret = unidir_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(many_short_loss)
        {
            int ret = many_short_loss_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_mtu_discovery)
        {
            int ret = mtu_discovery_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_spurious_retransmit)
        {
            int ret = spurious_retransmit_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_pn_ctr)
        {
            int ret = pn_ctr_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_tls_zero_share)
        {
            int ret = tls_zero_share_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(key_rotation_vector)
        {
            int ret = key_rotation_vector_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(draft17_vector)
        {
            int ret = draft17_vector_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(test_transport_param_log)
        {
            int ret = transport_param_log_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_bad_certificate)
        {
            int ret = bad_certificate_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_certificate_callback)
        {
            int ret = set_verify_certificate_callback_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_request_client_authentication)
        {
          int ret = request_client_authentication_test();

          Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_virtual_time)
        {
            int ret = virtual_time_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_different_params)
        {
            int ret = tls_different_params_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_quant_params)
        {
            int ret = tls_quant_params_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_set_certificate_and_key)
        {
            int ret = set_certificate_and_key_test();

            Assert::AreEqual(ret, 0);
        }
    
        TEST_METHOD(test_bad_client_certificate)
        {
            int ret = bad_client_certificate_test();

            Assert::AreEqual(ret, 0);
        }
    
        TEST_METHOD(test_nat_rebinding)
        {
            int ret = nat_rebinding_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_nat_rebinding_loss)
        {
            int ret = nat_rebinding_loss_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(fast_nat_rebinding)
        {
            int ret = fast_nat_rebinding_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_spin_bit)
        {
            int ret = spin_bit_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_client_error)
        {
            int ret = client_error_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_packet_enc_dec)
        {
            int ret = packet_enc_dec_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_pn_vector)
        {
            int ret = cleartext_pn_vector_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(zero_rtt_spurious)
        {
            int ret = zero_rtt_spurious_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(zero_rtt_retry)
        {
            int ret = zero_rtt_retry_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_transmit_cnxid)
        {
            int ret = transmit_cnxid_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(test_probe_api)
        {
            int ret = probe_api_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_migration)
        {
            int ret = migration_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_migration_long)
        {
            int ret = migration_test_long();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_migration_loss)
        {
            int ret = migration_test_loss();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(preferred_address)
        {
            int ret = preferred_address_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(test_cnxid_renewal)
        {
            int ret = cnxid_renewal_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_retire_cnxid)
        {
            int ret = retire_cnxid_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(not_before_cnxid)
        {
            int ret = not_before_cnxid_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(test_server_busy)
        {
            int ret = server_busy_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(initial_close)
        {
            int ret = initial_close_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(initial_server_close)
        {
            int ret = initial_server_close_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(new_rotated_key)
        {
            int ret = new_rotated_key_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(key_rotation)
        {
            int ret = key_rotation_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(key_rotation_stress)
        {
            int ret = key_rotation_stress_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(false_migration)
        {
            int ret = false_migration_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(nat_handshake)
        {
            int ret = nat_handshake_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(short_initial_cid)
        {
            int ret = short_initial_cid_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(stream_id_max)
        {
            int ret = stream_id_max_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(padding)
        {
            int ret = padding_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(packet_trace)
        {
            int ret = packet_trace_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(rebinding_stress)
        {
            int ret = rebinding_stress_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(ready_to_send)
        {
            int ret = ready_to_send_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(cubic)
        {
            int ret = cubic_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(cubic_jitter)
        {
            int ret = cubic_jitter_test();

            Assert::AreEqual(ret, 0);
        }
        TEST_METHOD(fastcc)
        {
            int ret = fastcc_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(fastcc_jitter)
        {
            int ret = fastcc_jitter_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(long_rtt)
        {
            int ret = long_rtt_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(cid_length)
        {
            int ret = cid_length_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(optimistic_ack)
        {
            int ret = optimistic_ack_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(optimistic_hole)
        {
            int ret = optimistic_hole_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(bad_coalesce)
        {
            int ret = bad_coalesce_test();

            Assert::AreEqual(ret, 0);
        }


        TEST_METHOD(document_addresses)
        {
            int ret = document_addresses_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(large_client_hello) {
            int ret = large_client_hello_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(stress)
        {
            int ret = stress_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(fuzz)
        {
            int ret = fuzz_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(fuzz_initial)
        {
            int ret = fuzz_initial_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(h3zero_integer) {
            int ret = h3zero_integer_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(qpack_huffman) {
            int ret = qpack_huffman_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(h3zero_parse_qpack) {
            int ret = h3zero_parse_qpack_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(h3zero_prepare_qpack) {
            int ret = h3zero_prepare_qpack_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(h3zero_stream) {
            int ret = h3zero_stream_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(parse_demo_scenario) {
            int ret = parse_demo_scenario_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(h3zero_server) {
            int ret = h3zero_server_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(h09_server) {
            int ret = h09_server_test();

            Assert::AreEqual(ret, 0);
        }
        
        TEST_METHOD(generic_server) {
            int ret = generic_server_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(esni) {
            int ret = esni_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(h3zero_post) {
            int ret = h3zero_post_test();

            Assert::AreEqual(ret, 0);
        }

        TEST_METHOD(h09_post) {
            int ret = h09_post_test();

            Assert::AreEqual(ret, 0);
        }
    };
}
