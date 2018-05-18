//  The MIT License
//  
//  Copyright (c) 2018 Chris Feyerchak
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef SUBNET_H
#define SUBNET_H

#include <cstdint>

namespace cfeyer {
namespace ip_coalesce {

uint32_t subnet_start_address( uint32_t subnet_address, uint32_t subnet_mask );
uint32_t subnet_end_address( uint32_t subnet_address, uint32_t subnet_mask );

constexpr int noncontiguous_subnet_mask = -1;
int count_contiguous_network_ones( uint32_t subnet_mask );

uint64_t netmask_length_to_address_count( int netmask_length_bits );

bool is_subnet( uint32_t address, uint64_t size );

bool is_power_of_2( uint64_t x );
int log_base_2( uint64_t x );

uint32_t size_to_subnet_mask( uint64_t size );


} // namespace ip_coalesce
} // namespace cfeyer

#endif /* SUBNET_H */
