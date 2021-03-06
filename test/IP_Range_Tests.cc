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

#include "gtest/gtest.h"

#include <sstream>

#include <cfeyer/ip_coalesce/IP_Range.h>
#include "Format.h"
#include "CIDR_Network.h"
#include "Interval.h"
#include <cfeyer/ip_coalesce/Coalescing_IP_Range_Set.h>


using namespace cfeyer::ip_coalesce;


TEST(Format, test_to_dotted_octet) {
   EXPECT_EQ( "0.0.0.0", to_dotted_octet(0x00000000) );
   EXPECT_EQ( "255.255.255.255", to_dotted_octet(0xffffffff) );
   EXPECT_EQ( "1.2.3.4", to_dotted_octet(0x01020304) );
}

#define EXPECT_IP_EQ(e,a) \
   { \
      EXPECT_EQ((e),(a)) \
                     << "Expected: " << to_dotted_octet((e)) << "\n" \
                     << "Actual: " << to_dotted_octet((a)) << "\n"; \
   }

TEST(Format, test_from_octets) {
   EXPECT_IP_EQ( 0x00000000, from_octets(0,0,0,0) );
   EXPECT_IP_EQ( 0x01020304, from_octets(1,2,3,4) );
   EXPECT_IP_EQ( 0xFF0000FF, from_octets(255,0,0,255) );
}

TEST(Subnet, test_count_contiguous_subnet_ones) {
   EXPECT_EQ( 0, count_contiguous_network_ones(0x00000000) );
   EXPECT_EQ( 24, count_contiguous_network_ones(0xffffff00) );
   EXPECT_EQ( 32, count_contiguous_network_ones(0xffffffff) );
   EXPECT_EQ( noncontiguous_subnet_mask, count_contiguous_network_ones(0xff0000ff) );
}

TEST(Subnet, test_netmask_length_to_address_count) {
   EXPECT_EQ( 1, netmask_length_to_address_count(32) );
   EXPECT_EQ( 256, netmask_length_to_address_count(24) );
   EXPECT_EQ( 65536, netmask_length_to_address_count(16) );
   EXPECT_EQ( 0x01000000, netmask_length_to_address_count(8) );
   EXPECT_EQ( 0x100000000, netmask_length_to_address_count(0) );
}

TEST(Subnet, test_start_address_0_bit_subnet) {
   EXPECT_IP_EQ( 0x00000000, subnet_start_address( from_octets(0,0,0,0), 0 ) );

   EXPECT_IP_EQ( 0x00000000, subnet_start_address( from_octets(255,0,0,0), 0 ) );
   EXPECT_IP_EQ( 0x00000000, subnet_start_address( from_octets(0,255,0,0), 0 ) );
   EXPECT_IP_EQ( 0x00000000, subnet_start_address( from_octets(0,0,255,0), 0 ) );
   EXPECT_IP_EQ( 0x00000000, subnet_start_address( from_octets(0,0,0,255), 0 ) );
}

TEST(Subnet, test_end_address_0_bit_subnet) {
   EXPECT_IP_EQ( 0xffffffff, subnet_end_address( from_octets(0,0,0,0), 0 ) );

   EXPECT_IP_EQ( 0xffffffff, subnet_end_address( from_octets(255,0,0,0), 0 ) );
   EXPECT_IP_EQ( 0xffffffff, subnet_end_address( from_octets(0,255,0,0), 0 ) );
   EXPECT_IP_EQ( 0xffffffff, subnet_end_address( from_octets(0,0,255,0), 0 ) );
   EXPECT_IP_EQ( 0xffffffff, subnet_end_address( from_octets(0,0,0,255), 0 ) );
}

TEST(Subnet, test_start_address_1_bit_subnet) {
   EXPECT_IP_EQ( 0x00000000, subnet_start_address( from_octets(0,0,0,0), 0x80000000 ) );
   EXPECT_IP_EQ( 0x80000000, subnet_start_address( from_octets(128,0,0,0), 0x80000000 ) );

   EXPECT_IP_EQ( 0x80000000, subnet_start_address( from_octets(255,0,0,0), 0x80000000 ) );
   EXPECT_IP_EQ( 0x00000000, subnet_start_address( from_octets(0,255,0,0), 0x80000000 ) );
   EXPECT_IP_EQ( 0x00000000, subnet_start_address( from_octets(0,0,255,0), 0x80000000 ) );
   EXPECT_IP_EQ( 0x00000000, subnet_start_address( from_octets(0,0,0,255), 0x80000000 ) );
}

TEST(Subnet, test_end_address_1_bit_subnet) {
   EXPECT_IP_EQ( 0x7fffffff, subnet_end_address( from_octets(0,0,0,0), 0x80000000 ) );

   EXPECT_IP_EQ( 0xffffffff, subnet_end_address( from_octets(255,0,0,0), 0x80000000 ) );
   EXPECT_IP_EQ( 0x7fffffff, subnet_end_address( from_octets(0,255,0,0), 0x80000000 ) );
   EXPECT_IP_EQ( 0x7fffffff, subnet_end_address( from_octets(0,0,255,0), 0x80000000 ) );
   EXPECT_IP_EQ( 0x7fffffff, subnet_end_address( from_octets(0,0,0,255), 0x80000000 ) );
}

TEST(Subnet, test_start_address_24_bit_subnet) {
   EXPECT_IP_EQ( from_octets(192,168,1,0), subnet_start_address( from_octets(192,168,1,0), from_octets(255,255,255,0) ) );
   EXPECT_IP_EQ( from_octets(10,20,30,0), subnet_start_address( from_octets(10,20,30,0), from_octets(255,255,255,0) ) );

   EXPECT_IP_EQ( from_octets(192,168,1,0), subnet_start_address( from_octets(192,168,1,1), from_octets(255,255,255,0) ) );
   EXPECT_IP_EQ( from_octets(192,168,1,0), subnet_start_address( from_octets(192,168,1,254), from_octets(255,255,255,0) ) );
}

TEST(Subnet, test_end_address_24_bit_subnet) {
   EXPECT_IP_EQ( from_octets(192,168,1,255), subnet_end_address( from_octets(192,168,1,0), from_octets(255,255,255,0) ) );
   EXPECT_IP_EQ( from_octets(10,20,30,255), subnet_end_address( from_octets(10,20,30,0), from_octets(255,255,255,0) ) );

   EXPECT_IP_EQ( from_octets(192,168,1,255), subnet_end_address( from_octets(192,168,1,1), from_octets(255,255,255,0) ) );
   EXPECT_IP_EQ( from_octets(192,168,1,255), subnet_end_address( from_octets(192,168,1,254), from_octets(255,255,255,0) ) );
}

TEST(Subnet, test_start_address_32_bit_subnet) {
   EXPECT_IP_EQ( from_octets(192,168,1,0), subnet_start_address( from_octets(192,168,1,0), from_octets(255,255,255,255) ) );
   EXPECT_IP_EQ( from_octets(10,20,30,40), subnet_start_address( from_octets(10,20,30,40), from_octets(255,255,255,255) ) );

   EXPECT_IP_EQ( from_octets(192,168,1,1), subnet_start_address( from_octets(192,168,1,1), from_octets(255,255,255,255) ) );
   EXPECT_IP_EQ( from_octets(192,168,1,254), subnet_start_address( from_octets(192,168,1,254), from_octets(255,255,255,255) ) );
}

TEST(Subnet, test_end_address_32_bit_subnet) {
   EXPECT_IP_EQ( from_octets(192,168,1,0), subnet_end_address( from_octets(192,168,1,0), from_octets(255,255,255,255) ) );
   EXPECT_IP_EQ( from_octets(10,20,30,40), subnet_end_address( from_octets(10,20,30,40), from_octets(255,255,255,255) ) );

   EXPECT_IP_EQ( from_octets(192,168,1,1), subnet_end_address( from_octets(192,168,1,1), from_octets(255,255,255,255) ) );
   EXPECT_IP_EQ( from_octets(192,168,1,254), subnet_end_address( from_octets(192,168,1,254), from_octets(255,255,255,255) ) );
}

TEST(Subnet, test_start_address_31_bit_subnet) {
   EXPECT_IP_EQ( from_octets(192,168,1,0), subnet_start_address( from_octets(192,168,1,0), from_octets(255,255,255,254) ) );
   EXPECT_IP_EQ( from_octets(10,20,30,40), subnet_start_address( from_octets(10,20,30,40), from_octets(255,255,255,254) ) );

   EXPECT_IP_EQ( from_octets(192,168,1,0), subnet_start_address( from_octets(192,168,1,1), from_octets(255,255,255,254) ) );
}

TEST(Subnet, test_end_address_31_bit_subnet) {
   EXPECT_IP_EQ( from_octets(192,168,1,1), subnet_end_address( from_octets(192,168,1,0), from_octets(255,255,255,254) ) );
   EXPECT_IP_EQ( from_octets(10,20,30,41), subnet_end_address( from_octets(10,20,30,40), from_octets(255,255,255,254) ) );

   EXPECT_IP_EQ( from_octets(192,168,1,1), subnet_end_address( from_octets(192,168,1,1), from_octets(255,255,255,254) ) );
}

TEST(Subnet, test_is_power_of_2) {
   EXPECT_FALSE( is_power_of_2(0) );
   EXPECT_TRUE( is_power_of_2(1) );
   EXPECT_TRUE( is_power_of_2(2) );
   EXPECT_FALSE( is_power_of_2(3) );
   EXPECT_FALSE( is_power_of_2(0xffffffff) );
   EXPECT_TRUE( is_power_of_2(0x100000000) );
}

TEST(Subnet, test_log_base_2_for_powers_of_2) {
   EXPECT_EQ( 0, log_base_2(1) ); 
   EXPECT_EQ( 1, log_base_2(2) ); 
   EXPECT_EQ( 2, log_base_2(4) ); 
   EXPECT_EQ( 3, log_base_2(8) ); 
   EXPECT_EQ( 8, log_base_2(256) ); 
   EXPECT_EQ( 32, log_base_2(0x100000000) ); 
}

TEST(Subnet, test_log_base_2_throws_exception_if_arg_not_power_of_2) {
   EXPECT_ANY_THROW( log_base_2(0) );
   EXPECT_ANY_THROW( log_base_2(3) );
   EXPECT_ANY_THROW( log_base_2(0xffffffff) );
}

TEST(Subnet, test_size_to_subnet_mask_for_sizes_that_are_powers_of_two) {
   EXPECT_EQ( 0xffffffff, size_to_subnet_mask(1) );
   EXPECT_EQ( 0xfffffffe, size_to_subnet_mask(2) );
   EXPECT_EQ( 0xffffff00, size_to_subnet_mask(256) );
   EXPECT_EQ( 0x80000000, size_to_subnet_mask(2147483648) );
   EXPECT_EQ( 0x00000000, size_to_subnet_mask(4294967296) );
}

TEST(Subnet, test_size_to_subnet_mask_throws_exception_for_sizes_that_are_not_powers_of_two) {
   EXPECT_ANY_THROW( size_to_subnet_mask( 3 ) );
   EXPECT_ANY_THROW( size_to_subnet_mask( 0xffffffff ) );
}

TEST(Subnet, test_size_to_subnet_mask_throws_exception_for_sizes_larger_than_2_exp_32) {
   EXPECT_ANY_THROW( size_to_subnet_mask( 0x1000000000 ) );
}

TEST(Subnet, test_is_subnet_true_when_size_is_power_of_two_and_start_address_aligned_with_boundary) {
   EXPECT_TRUE( is_subnet(from_octets(192,168,1,0), 256) );
   EXPECT_TRUE( is_subnet(from_octets(0,0,0,0), 0x100000000) );
   EXPECT_TRUE( is_subnet(from_octets(192,168,1,23), 1) );
}

TEST(Subnet, test_is_subnet_false_when_size_is_power_of_two_and_start_address_not_aligned_with_boundary) {
   EXPECT_FALSE( is_subnet(from_octets(192,168,1,1), 256) );
   EXPECT_FALSE( is_subnet(from_octets(1,0,0,0), 0x100000000) );
}

TEST(Subnet, test_is_subnet_false_when_size_is_not_power_of_two_and_start_address_aligned_with_boundary) {
   EXPECT_FALSE( is_subnet(from_octets(192,168,1,0), 255) );
}

TEST(Subnet, test_is_subnet_false_when_size_is_not_power_of_two_and_start_address_not_aligned_with_boundary) {
   EXPECT_FALSE( is_subnet(from_octets(192,168,1,1), 255) );
}

TEST(Interval_Is_On_Or_Adjacent, test_throw_if_bounds_are_backwards) {
   EXPECT_ANY_THROW( is_on_or_adjacent( 2, 3, 1 ) );
}

TEST(Interval_Is_On_Or_Adjacent, test_far_below_interval) {
   EXPECT_FALSE( is_on_or_adjacent( 0, 2, 2 ) );
   EXPECT_FALSE( is_on_or_adjacent( 0, 2, 100 ) );
   EXPECT_FALSE( is_on_or_adjacent( 0, 2, 0xffffffff ) );
}

TEST(Interval_Is_On_Or_Adjacent, test_just_below_interval) {
   EXPECT_TRUE( is_on_or_adjacent( 0, 1, 1 ) );
   EXPECT_TRUE( is_on_or_adjacent( 0, 1, 100 ) );
   EXPECT_TRUE( is_on_or_adjacent( 0, 1, 0xffffffff ) );
}

TEST(Interval_Is_On_Or_Adjacent, test_equals_interval_lower_bound) {
   EXPECT_TRUE( is_on_or_adjacent( 1, 1, 1 ) );
   EXPECT_TRUE( is_on_or_adjacent( 1, 1, 100 ) );
   EXPECT_TRUE( is_on_or_adjacent( 0, 0, 0xffffffff ) );
}

TEST(Interval_Is_On_Or_Adjacent, test_exclusively_within_interval_bounds) {
   EXPECT_TRUE( is_on_or_adjacent( 1, 0, 3 ) );
   EXPECT_TRUE( is_on_or_adjacent( 50, 1, 100 ) );
   EXPECT_TRUE( is_on_or_adjacent( 42, 0, 0xffffffff ) );
}

TEST(Interval_Is_On_Or_Adjacent, test_equals_interval_upper_bound) {
   EXPECT_TRUE( is_on_or_adjacent( 1, 1, 1 ) );
   EXPECT_TRUE( is_on_or_adjacent( 100, 1, 100 ) );
   EXPECT_TRUE( is_on_or_adjacent( 0xffffffff, 0, 0xffffffff ) );
}

TEST(Interval_Is_On_Or_Adjacent, test_just_above_interval) {
   EXPECT_TRUE( is_on_or_adjacent( 2, 1, 1 ) );
   EXPECT_TRUE( is_on_or_adjacent( 101, 1, 100 ) );
   EXPECT_TRUE( is_on_or_adjacent( 0xffffffff, 0, 0xfffffffe ) );
}

TEST(Interval_Is_On_Or_Adjacent, test_far_above_interval) {
   EXPECT_FALSE( is_on_or_adjacent( 3, 1, 1 ) );
   EXPECT_FALSE( is_on_or_adjacent( 102, 1, 100 ) );
   EXPECT_FALSE( is_on_or_adjacent( 0xffffffff, 0, 0xfffffffd ) );
}

TEST(IP_Range, test_to_string) {
   EXPECT_EQ( "0.0.0.0", IP_Range(0x00000000, 0xffffffff).to_string() );
   EXPECT_EQ( "0.0.0.0/31", IP_Range(0x00000000, 0xfffffffe).to_string() );
   EXPECT_EQ( "0.0.0.0/0", IP_Range(0x00000000, 0x00000000).to_string() );
   EXPECT_EQ( "10.42.0.0/16", IP_Range(from_octets(10,42,0,0),from_octets(255,255,0,0)).to_string() );
   EXPECT_EQ( "192.168.1.0/24", IP_Range(from_octets(192,168,1,0),from_octets(255,255,255,0)).to_string() );
   EXPECT_EQ( "192.168.1.0-192.168.2.255",
              (IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) +
               IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0))).to_string() );
}

TEST(IP_Range, test_to_string_with_discontiguous_subnet_mask) {
   EXPECT_EQ( "0.0.0.0/255.255.0.255", IP_Range(0x00000000, 0xffff00ff).to_string() );
   EXPECT_EQ( "192.168.1.0/255.128.255.0", IP_Range( from_octets(192,168,1,0), from_octets(255,128,255,0)).to_string() );
}

TEST(IP_Range, test_is_coalescable_with_same_valued_range) {
   EXPECT_TRUE( IP_Range(0,0).is_coalescable( IP_Range(0,0) ) );
   EXPECT_TRUE( IP_Range(0xffff,0xffffffff).is_coalescable( IP_Range(0xffff,0xffffffff) ) );
   EXPECT_TRUE( IP_Range(0xffffffff,0xffffffff).is_coalescable( IP_Range(0xffffffff,0xffffffff) ) );
}

TEST(IP_Range, test_range_of_size_one_is_not_coalescable_with_a_far_below_range_of_size_one) {
   EXPECT_FALSE( IP_Range(2,0xffffffff).is_coalescable( IP_Range(0, 0xffffffff) ) );
   EXPECT_FALSE( IP_Range(44,0xffffffff).is_coalescable( IP_Range(42, 0xffffffff) ) );
   EXPECT_FALSE( IP_Range(0xffffffff,0xffffffff).is_coalescable( IP_Range(0xfffffffd, 0xffffffff) ) );
}

TEST(IP_Range, test_range_of_size_one_is_coalescable_with_a_just_below_range_of_size_one) {
   EXPECT_TRUE( IP_Range(1,0xffffffff).is_coalescable( IP_Range(0, 0xffffffff) ) );
   EXPECT_TRUE( IP_Range(43,0xffffffff).is_coalescable( IP_Range(42, 0xffffffff) ) );
   EXPECT_TRUE( IP_Range(0xffffffff,0xffffffff).is_coalescable( IP_Range(0xfffffffe, 0xffffffff) ) );
}

TEST(IP_Range, test_range_of_size_one_is_coalescable_with_a_just_above_range_of_size_one) {
   EXPECT_TRUE( IP_Range(0,0xffffffff).is_coalescable( IP_Range(1, 0xffffffff) ) );
   EXPECT_TRUE( IP_Range(42,0xffffffff).is_coalescable( IP_Range(43, 0xffffffff) ) );
   EXPECT_TRUE( IP_Range(0xfffffffe,0xffffffff).is_coalescable( IP_Range(0xffffffff, 0xffffffff) ) );
}

TEST(IP_Range, test_range_of_size_one_is_not_coalescable_with_a_far_above_range_of_size_one) {
   EXPECT_FALSE( IP_Range(0,0xffffffff).is_coalescable( IP_Range(2, 0xffffffff) ) );
   EXPECT_FALSE( IP_Range(42,0xffffffff).is_coalescable( IP_Range(44, 0xffffffff) ) );
   EXPECT_FALSE( IP_Range(0xfffffffd,0xffffffff).is_coalescable( IP_Range(0xffffffff, 0xffffffff) ) );
}

TEST(IP_Range, test_range_at_beginning_of_address_space_does_not_coalesce_with_range_at_end_of_address_space ) {
   EXPECT_FALSE( IP_Range(from_octets(255,255,255,255), from_octets(255,255,255,255)).is_coalescable(
                 IP_Range(from_octets(0,0,0,0), from_octets(255,255,255,255))) );

   EXPECT_FALSE( IP_Range(from_octets(0,0,0,0), from_octets(255,255,255,255)).is_coalescable(
                 IP_Range(from_octets(255,255,255,255), from_octets(255,255,255,255))) );
}

TEST(IP_Range, test_range_with_discontiguous_subnet_mask_is_not_coalescable_with_itself )
{
   EXPECT_FALSE( IP_Range(0, 0xffffff0f).is_coalescable( IP_Range(0, 0xffffff0f) ) );
}

TEST(IP_Range, test_range_with_discontiguous_subnet_mask_is_not_coalescable_with_encompassing_subnet )
{
   EXPECT_FALSE( IP_Range(0, 0xffffff0f).is_coalescable( IP_Range(0, 0xffffff00) ) );
   EXPECT_FALSE( IP_Range(0, 0xffffff00).is_coalescable( IP_Range(0, 0xffffff0f) ) );
}

TEST(IP_Range, test_same_address_and_same_netmask_are_equal) {
   EXPECT_TRUE( IP_Range(0,0) == IP_Range(0,0) );
}

TEST(IP_Range, test_same_address_and_different_netmask_are_not_equal) {
   EXPECT_FALSE( IP_Range(0,from_octets(255,255,255,255)) == IP_Range(0,from_octets(255,255,255,254)) );
}

TEST(IP_Range, test_different_address_and_same_netmask_are_not_equal) {
   EXPECT_FALSE( IP_Range(1,from_octets(255,255,255,254)) == IP_Range(2,from_octets(255,255,255,254)) );
}

TEST(IP_Range, test_different_address_and_different_netmask_are_not_equal) {
   EXPECT_FALSE( IP_Range(0,from_octets(255,255,255,255)) == IP_Range(1,from_octets(255,255,255,254)) );
}

TEST(IP_Range, test_same_address_and_same_netmask_are_not_less_than) {
   EXPECT_FALSE( IP_Range(0,0) < IP_Range(0,0) );
}

TEST(IP_Range, test_less_than_gives_precedence_to_start_address_over_subnet_size) {
   EXPECT_TRUE( IP_Range(0,0xffff0000) < IP_Range(from_octets(128,0,0,0), 0xffffff00) );
   EXPECT_FALSE( IP_Range(from_octets(128,0,0,0),0xffffff00) < IP_Range(0, 0xffff0000) );
}

TEST(IP_Range, test_less_than_defers_to_subnet_size_given_equal_start_address) {
   EXPECT_TRUE( IP_Range(0,0xffffff00) < IP_Range(0, 0xffff0000) );
   EXPECT_FALSE( IP_Range(0,0xffff0000) < IP_Range(0, 0xffffff00) );
}

TEST(IP_Range, test_coalescing_uncoalescable_ranges_throws_exception) {
   EXPECT_ANY_THROW( IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) +
                     IP_Range(from_octets(192,168,3,0), from_octets(255,255,255,0)) );
}

TEST(IP_Range, test_coalesce_with_same_valued_range) {
   EXPECT_EQ( IP_Range(0,0), IP_Range(0,0) + IP_Range(0,0) );
   EXPECT_EQ( IP_Range(0xffff, 0xffffffff), IP_Range(0xffff,0xffffffff) + IP_Range(0xffff,0xffffffff) );
   EXPECT_EQ( IP_Range(0xffffffff, 0xffffffff), IP_Range(0xffffffff,0xffffffff) + IP_Range(0xffffffff,0xffffffff) );

   {
      IP_Range r(0,0);
      r += IP_Range(0,0);
      EXPECT_EQ( IP_Range(0,0), r );
   }
}

TEST(IP_Range, test_coalesce_with_adjacent_range) {
   EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,254,0)),
                 IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) +
                 IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,254,0)),
                 IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) +
                 IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) );

   {
      IP_Range r(from_octets(192,168,0,0), from_octets(255,255,255,0));
      r += IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0));
      EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,254,0)), r );
   }
}

TEST(IP_Range, test_coalesce_with_subset_range) {
   EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,254,0)),
                 IP_Range(from_octets(192,168,0,0), from_octets(255,255,254,0)) +
                 IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,254,0)),
                 IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) +
                 IP_Range(from_octets(192,168,0,0), from_octets(255,255,254,0)) );
}

TEST(IP_Range, test_sloppy_subnet_addresses_are_equivalent_to_subnet_boundary_addresses) {
   EXPECT_EQ( IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)),
              IP_Range(from_octets(192,168,1,1), from_octets(255,255,255,0)) );

   EXPECT_EQ( IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)),
              IP_Range(from_octets(192,168,1,255), from_octets(255,255,255,0)) );

   EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,0,0)),
              IP_Range(from_octets(192,168,1,1), from_octets(255,255,0,0)) );

   EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,0,0)),
              IP_Range(from_octets(192,168,255,255), from_octets(255,255,0,0)) );
}

TEST(IP_Range, test_size_after_construction) {
   EXPECT_EQ( 1, IP_Range(from_octets(192,168,1,1), from_octets(255,255,255,255)).size() );
   EXPECT_EQ( 256, IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)).size() );
   EXPECT_EQ( 65536, IP_Range(from_octets(192,168,0,0), from_octets(255,255,0,0)).size() );
   EXPECT_EQ( 0x100000000, IP_Range(from_octets(0,0,0,0), from_octets(0,0,0,0)).size() );

   EXPECT_EQ( 1, IP_Range(from_octets(192,168,1,1), from_octets(255,255,255,255)).size() );
   EXPECT_EQ( 256, IP_Range(from_octets(192,168,1,1), from_octets(255,255,255,0)).size() );
   EXPECT_EQ( 65536, IP_Range(from_octets(192,168,0,1), from_octets(255,255,0,0)).size() );
   EXPECT_EQ( 0x100000000, IP_Range(from_octets(0,0,0,1), from_octets(0,0,0,0)).size() );
}

TEST(IP_Range, test_size_after_coalescing_with_equivalent_range) {
   EXPECT_EQ( 256,
              (IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) +
               IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0))).size() );

   EXPECT_EQ( 256,
              (IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) +
               IP_Range(from_octets(192,168,1,128), from_octets(255,255,255,0))).size() );
}

TEST(IP_Range, test_size_after_coalescing_with_nested_range) {
   EXPECT_EQ( 256,
              (IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) +
               IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,254))).size() );

   EXPECT_EQ( 256,
              (IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,254)) +
               IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0))).size() );
}

TEST(IP_Range, test_size_after_coalescing_with_neighbor) {
   EXPECT_EQ( 512,
              (IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) +
               IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0))).size() );
}

TEST(IP_Range, test_is_subnet_is_true_for_contiguous_subnet_masks) {
   EXPECT_TRUE( IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)).is_subnet() );
   EXPECT_TRUE( IP_Range(from_octets(192,168,1,1), from_octets(255,255,255,0)).is_subnet() );

   EXPECT_TRUE( IP_Range(from_octets(192,168,1,0), from_octets(255,255,0,0)).is_subnet() );
   EXPECT_TRUE( IP_Range(from_octets(192,168,1,1), from_octets(255,255,0,0)).is_subnet() );
}

TEST(IP_Range, test_is_subnet_is_false_for_noncontiguous_subnet_masks) {
   EXPECT_FALSE( IP_Range(from_octets(192,168,1,0), from_octets(255,0,255,0)).is_subnet() );
}

TEST(IP_Range, test_is_subnet_is_true_after_coalescing_sibling_subnets) {
   EXPECT_TRUE( (IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) +
                 IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0))).is_subnet() );

   EXPECT_TRUE( (IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0)) +
                 IP_Range(from_octets(192,168,3,0), from_octets(255,255,255,0))).is_subnet() );
}

TEST(IP_Range, test_is_subnet_is_false_after_coalescing_nonsibling_subnets) {
   EXPECT_FALSE( (IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) +
                  IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0))).is_subnet() );
}

TEST(IP_Range, test_from_string_of_empty_string_throws_exception) {
   IP_Range range;
   EXPECT_ANY_THROW( range.from_string( "" ) );
}

TEST(IP_Range, test_from_string_0_0_0_0_slash_255_255_255_255) {
   IP_Range range;
   range.from_string( "0.0.0.0/255.255.255.255" );
   EXPECT_TRUE( IP_Range(from_octets(0,0,0,0), from_octets(255,255,255,255)) == range );
}

TEST(IP_Range, test_from_string_0_0_0_0_slash_0_0_0_0) {
   IP_Range range;
   range.from_string( "0.0.0.0/0.0.0.0" );
   EXPECT_TRUE( IP_Range(from_octets(0,0,0,0), from_octets(0,0,0,0)) == range );
}

TEST(IP_Range, test_from_string_255_255_255_255_slash_255_255_255_255) {
   IP_Range range;
   range.from_string( "255.255.255.255/255.255.255.255" );
   EXPECT_TRUE( IP_Range(from_octets(255,255,255,255), from_octets(255,255,255,255)) == range );
}

TEST(IP_Range, test_from_string_192_168_1_2_slash_255_255_255_0) {
   IP_Range range;
   range.from_string( "192.168.1.2/255.255.255.0" );
   EXPECT_TRUE( IP_Range(from_octets(192,168,1,2), from_octets(255,255,255,0)) == range );
}

TEST(IP_Range, test_from_string_192_168_1_2) {
   IP_Range range;
   range.from_string( "192.168.1.2" );
   EXPECT_TRUE( IP_Range(from_octets(192,168,1,2), from_octets(255,255,255,255)) == range );
}

TEST(IP_Range, test_from_string_192_168_0_0_dash_192_168_1_255) {
   IP_Range range;
   range.from_string( "192.168.0.0-192.168.1.255" );
   EXPECT_TRUE( IP_Range(from_octets(192,168,0,0), from_octets(255,255,254,0)) == range );
}

TEST(IP_Range, test_stream_input_0_0_0_0_slash_255_255_255_255) {
   std::istringstream strm( "0.0.0.0/255.255.255.255" );
   IP_Range range;
   strm >> range;
   EXPECT_TRUE( IP_Range(from_octets(0,0,0,0), from_octets(255,255,255,255)) == range );
}

TEST(IP_Range, test_stream_input_0_0_0_0_slash_0_0_0_0) {
   std::istringstream strm( "0.0.0.0/0.0.0.0" );
   IP_Range range;
   strm >> range;
   EXPECT_TRUE( IP_Range(from_octets(0,0,0,0), from_octets(0,0,0,0)) == range );
}

TEST(IP_Range, test_stream_input_255_255_255_255_slash_255_255_255_255) {
   std::istringstream strm( "255.255.255.255/255.255.255.255" );
   IP_Range range;
   strm >> range;
   EXPECT_TRUE( IP_Range(from_octets(255,255,255,255), from_octets(255,255,255,255)) == range );
}

TEST(IP_Range, test_stream_input_192_168_1_2_slash_255_255_255_0) {
   std::istringstream strm( "192.168.1.2/255.255.255.0" );
   IP_Range range;
   strm >> range;
   EXPECT_TRUE( IP_Range(from_octets(192,168,1,2), from_octets(255,255,255,0)) == range );
}

TEST(IP_Range, test_stream_input_192_168_1_2_slash_0) {
   std::istringstream strm( "192.168.1.2/0" );
   IP_Range range;
   strm >> range;
   EXPECT_TRUE( IP_Range(from_octets(192,168,1,2), from_octets(0,0,0,0)) == range );
}

TEST(IP_Range, test_stream_input_192_168_1_2_slash_32) {
   std::istringstream strm( "192.168.1.2/32" );
   IP_Range range;
   strm >> range;
   EXPECT_TRUE( IP_Range(from_octets(192,168,1,2), from_octets(255,255,255,255)) == range );
}

TEST(IP_Range, test_stream_input_from_empty_stream) {
   std::istringstream strm( "192.168.1.2/255.255.255.0" );

   {
      IP_Range range;
      EXPECT_TRUE( strm >> range );
      EXPECT_TRUE( IP_Range(from_octets(192,168,1,2), from_octets(255,255,255,0)) == range );
   }

   {
      IP_Range range;
      EXPECT_FALSE( strm >> range );
   }
}

TEST(IP_Range, test_stream_input_three_space_delimited_ranges) {
   std::istringstream strm( "0.0.0.0/0.0.0.0 255.255.255.255/255.255.255.255 192.168.1.2/255.255.255.0" );

   {
      IP_Range range;
      strm >> range;
      EXPECT_TRUE( IP_Range(from_octets(0,0,0,0), from_octets(0,0,0,0)) == range );
   }

   {
      IP_Range range;
      strm >> range;
      EXPECT_TRUE( IP_Range(from_octets(255,255,255,255), from_octets(255,255,255,255)) == range );
   }

   {
      IP_Range range;
      strm >> range;
      EXPECT_TRUE( IP_Range(from_octets(192,168,1,2), from_octets(255,255,255,0)) == range );
   }
}

TEST(IP_Range, test_stream_output) {
   {
      std::ostringstream strm;
      strm << IP_Range(0x00000000, 0xffffffff);
      EXPECT_EQ( "0.0.0.0", strm.str() );
   }
   {
      std::ostringstream strm;
      strm << IP_Range(0x00000000, 0x00000000);
      EXPECT_EQ( "0.0.0.0/0", strm.str() );
   }
   {
      std::ostringstream strm;
      strm << IP_Range( from_octets(192,168,1,0), from_octets(255,255,255,0) );
      EXPECT_EQ( "192.168.1.0/24", strm.str() );
   }
   {
      std::ostringstream strm;
      strm << IP_Range( from_octets(192,168,1,0), from_octets(255,255,255,0) ) +
              IP_Range( from_octets(192,168,2,0), from_octets(255,255,255,0) );
      EXPECT_EQ( "192.168.1.0-192.168.2.255", strm.str() );
   }
}

TEST(Coalescing_IP_Range_Set, test_add_two_presorted_uncoalescable_ranges ) {
   Coalescing_IP_Range_Set set;
   EXPECT_EQ( 0, set.size() );

   set.insert( IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( 2, set.size() );

   auto iter = set.begin();
   EXPECT_TRUE( IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) == *iter );
   iter++;
   EXPECT_TRUE( IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0)) == *iter );
}

TEST(Coalescing_IP_Range_Set, test_add_two_presorted_coalescable_ranges ) {
   Coalescing_IP_Range_Set set;
   EXPECT_EQ( 0, set.size() );

   set.insert( IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( 1, set.size() );

   EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,254,0)), *set.begin() );
}

TEST(Coalescing_IP_Range_Set, test_add_two_unsorted_coalescable_ranges ) {
   Coalescing_IP_Range_Set set;
   EXPECT_EQ( 0, set.size() );

   set.insert( IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( 1, set.size() );

   EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,254,0)), *set.begin() );
}

TEST(Coalescing_IP_Range_Set, test_add_two_presorted_uncoalescable_ranges_then_add_missing_range ) {
   Coalescing_IP_Range_Set set;
   EXPECT_EQ( 0, set.size() );

   set.insert( IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( 2, set.size() );

   set.insert( IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( 1, set.size() );

   EXPECT_EQ( "192.168.0.0-192.168.2.255", set.begin()->to_string() );
}

TEST(Coalescing_IP_Range_Set, test_add_three_presorted_uncoalescable_ranges_then_add_one_missing_range ) {
   Coalescing_IP_Range_Set set;
   EXPECT_EQ( 0, set.size() );

   set.insert( IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,4,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( 3, set.size() );

   set.insert( IP_Range(from_octets(192,168,1,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( 2, set.size() );
}

TEST(Coalescing_IP_Range_Set, test_add_three_presorted_uncoalescable_ranges_then_add_range_that_spans_over_all ) {
   Coalescing_IP_Range_Set set;
   EXPECT_EQ( 0, set.size() );

   set.insert( IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,4,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( 3, set.size() );

   set.insert( IP_Range(from_octets(192,168,0,0), from_octets(255,255,0,0)) );

   EXPECT_EQ( 1, set.size() );

   EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,0,0)), *set.begin() );
}

TEST(Coalescing_IP_Range_Set, test_add_four_presorted_uncoalescable_ranges_then_add_range_that_spans_over_most ) {
   Coalescing_IP_Range_Set set;
   EXPECT_EQ( 0, set.size() );

   set.insert( IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,4,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(255,255,255,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( 4, set.size() );

   set.insert( IP_Range(from_octets(192,168,0,0), from_octets(255,255,0,0)) );

   EXPECT_EQ( 2, set.size() );

   auto iter = set.begin();
   EXPECT_EQ( IP_Range(from_octets(192,168,0,0), from_octets(255,255,0,0)), *iter );
   iter++;
   EXPECT_EQ( IP_Range(from_octets(255,255,255,0), from_octets(255,255,255,0)), *iter );
}

TEST(Coalescing_IP_Range_Set, test_add_two_unsorted_uncoalescable_ranges ) {
   Coalescing_IP_Range_Set set;
   EXPECT_EQ( 0, set.size() );

   set.insert( IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0)) );
   set.insert( IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) );

   EXPECT_EQ( 2, set.size() );

   auto iter = set.begin();
   EXPECT_TRUE( IP_Range(from_octets(192,168,0,0), from_octets(255,255,255,0)) == *iter );
   iter++;
   EXPECT_TRUE( IP_Range(from_octets(192,168,2,0), from_octets(255,255,255,0)) == *iter );
}
