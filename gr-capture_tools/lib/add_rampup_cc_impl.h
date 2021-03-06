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

#ifndef INCLUDED_CAPTURE_TOOLS_ADD_RAMPUP_CC_IMPL_H
#define INCLUDED_CAPTURE_TOOLS_ADD_RAMPUP_CC_IMPL_H

#include <capture_tools/add_rampup_cc.h>

namespace gr {
  namespace capture_tools {

    class add_rampup_cc_impl : public add_rampup_cc
    {
     private:
      // Nothing to declare in this block.
        float d_scale;
        float d_alpha;
        int d_rampcounter;
        int d_state;
        int d_constant_start;
        int d_constant_stop;
        int d_constant_length;
        float d_startval_rampdown;

     public:
      add_rampup_cc_impl(float alpha, int constant_start, int constant_stop, int constant_length);
      ~add_rampup_cc_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace capture_tools
} // namespace gr

#endif /* INCLUDED_CAPTURE_TOOLS_ADD_RAMPUP_CC_IMPL_H */

