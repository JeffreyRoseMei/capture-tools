/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "annotated_to_msg_f_impl.h"

namespace gr {
  namespace capture_tools {

    annotated_to_msg_f::sptr
    annotated_to_msg_f::make(gr::msg_queue::sptr packet_queue)
    {
      return gnuradio::get_initial_sptr
        (new annotated_to_msg_f_impl(packet_queue));
    }

    /*
     * The private constructor
     */
    annotated_to_msg_f_impl::annotated_to_msg_f_impl(gr::msg_queue::sptr packet_queue)
      : gr::sync_block("annotated_to_msg_f",
              gr::io_signature::make(2, 2, sizeof(float)),
              gr::io_signature::make(0, 0, 0))
    {
        d_state = 0;
        d_packet_queue = packet_queue;
    }

    /*
     * Our virtual destructor.
     */
    annotated_to_msg_f_impl::~annotated_to_msg_f_impl()
    {
    }

    int
    annotated_to_msg_f_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        std::vector<tag_t> tags;
        int tagnum = -1;
        int nextstrobe = -1;
      const float *in = (const float *) input_items[0];
      const float *in_th = (const float *) input_items[1];

        get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items);
        for(int i=tagnum+1;i<tags.size();i++){
            if (pmt::equal(tags[i].key, pmt::intern("strobe"))) {
                nextstrobe = tags[i].offset - nitems_read(0);
                tagnum = i;
                break;
            }
        }

      // Do <+signal processing+>
        for(int i=0;i<noutput_items;i++) {
            if (d_state == 0) {
                if (in_th[i] == 1) {
                    d_state = 1;
                    d_receive_buffer.clear();
                }
            }
            else if (d_state == 1) {
                if (in_th[i] == 0) {
                    d_state = 0;
                    printf("Received: ");
                    for(int j=0;j<d_receive_buffer.size();j++){
                        printf("%d", d_receive_buffer[j]);
                    }
                    printf("\n");
                }
            }
            if(d_state == 1) {
                if((nextstrobe != -1) && (i == nextstrobe)) {
                    d_receive_buffer.push_back((in[i]) > 0.0 ? 1 : 0);
                    nextstrobe = -1;
                    for(int i=tagnum+1;i<tags.size();i++){
                        if (pmt::equal(tags[i].key, pmt::intern("strobe"))) {
                            nextstrobe = tags[i].offset - nitems_read(0);
                            tagnum = i;
                            break;
                        }
                    }
                }
            }
        }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace capture_tools */
} /* namespace gr */
