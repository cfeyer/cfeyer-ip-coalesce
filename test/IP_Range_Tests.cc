#include "gtest/gtest.h"

#include "IP_Range.h"
#include "Format.h"
#include "Subnet.h"
#include "Interval.h"

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

TEST(Format, test_octets_to_uint32) {
   EXPECT_IP_EQ( 0x00000000, octets_to_uint32(0,0,0,0) );
   EXPECT_IP_EQ( 0x01020304, octets_to_uint32(1,2,3,4) );
   EXPECT_IP_EQ( 0xFF0000FF, octets_to_uint32(255,0,0,255) );
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
   EXPECT_IP_EQ( 0x00000000, subnet_start_address( octets_to_uint32(0,0,0,0), 0 ) );
}

TEST(Subnet, test_end_address_0_bit_subnet) {
   EXPECT_IP_EQ( 0xffffffff, subnet_end_address( octets_to_uint32(0,0,0,0), 0 ) );
}

TEST(Subnet, test_start_address_1_bit_subnet) {
   EXPECT_IP_EQ( 0x00000000, subnet_start_address( octets_to_uint32(0,0,0,0), 0x80000000 ) );
   EXPECT_IP_EQ( 0xfffffffe, subnet_start_address( octets_to_uint32(255,255,255,254), 0x80000000 ) );
}

TEST(Subnet, test_end_address_1_bit_subnet) {
   EXPECT_IP_EQ( 0x7fffffff, subnet_end_address( octets_to_uint32(0,0,0,0), 0x80000000 ) );
}

TEST(Subnet, test_start_address_24_bit_subnet) {
   EXPECT_IP_EQ( octets_to_uint32(192,168,1,0), subnet_start_address( octets_to_uint32(192,168,1,0), octets_to_uint32(255,255,255,0) ) );
   EXPECT_IP_EQ( octets_to_uint32(10,20,30,0), subnet_start_address( octets_to_uint32(10,20,30,0), octets_to_uint32(255,255,255,0) ) );
}

TEST(Subnet, test_end_address_24_bit_subnet) {
   EXPECT_IP_EQ( octets_to_uint32(192,168,1,255), subnet_end_address( octets_to_uint32(192,168,1,0), octets_to_uint32(255,255,255,0) ) );
   EXPECT_IP_EQ( octets_to_uint32(10,20,30,255), subnet_end_address( octets_to_uint32(10,20,30,0), octets_to_uint32(255,255,255,0) ) );
}

TEST(Subnet, test_start_address_32_bit_subnet) {
   EXPECT_IP_EQ( octets_to_uint32(192,168,1,0), subnet_start_address( octets_to_uint32(192,168,1,0), octets_to_uint32(255,255,255,255) ) );
   EXPECT_IP_EQ( octets_to_uint32(10,20,30,40), subnet_start_address( octets_to_uint32(10,20,30,40), octets_to_uint32(255,255,255,255) ) );
}

TEST(Subnet, test_end_address_32_bit_subnet) {
   EXPECT_IP_EQ( octets_to_uint32(192,168,1,0), subnet_end_address( octets_to_uint32(192,168,1,0), octets_to_uint32(255,255,255,255) ) );
   EXPECT_IP_EQ( octets_to_uint32(10,20,30,40), subnet_end_address( octets_to_uint32(10,20,30,40), octets_to_uint32(255,255,255,255) ) );
}

TEST(Subnet, test_start_address_31_bit_subnet) {
   EXPECT_IP_EQ( octets_to_uint32(192,168,1,0), subnet_start_address( octets_to_uint32(192,168,1,0), octets_to_uint32(255,255,255,254) ) );
   EXPECT_IP_EQ( octets_to_uint32(10,20,30,40), subnet_start_address( octets_to_uint32(10,20,30,40), octets_to_uint32(255,255,255,254) ) );
}

TEST(Subnet, test_end_address_31_bit_subnet) {
   EXPECT_IP_EQ( octets_to_uint32(192,168,1,1), subnet_end_address( octets_to_uint32(192,168,1,0), octets_to_uint32(255,255,255,254) ) );
   EXPECT_IP_EQ( octets_to_uint32(10,20,30,41), subnet_end_address( octets_to_uint32(10,20,30,40), octets_to_uint32(255,255,255,254) ) );
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

TEST(IP_Range, test_to_dotted_octet) {
   EXPECT_EQ( "0.0.0.0", IP_Range(0x00000000, 0xffffffff).to_dotted_octet() );
   EXPECT_EQ( "0.0.0.0-255.255.255.255", IP_Range(0x00000000, 0x00000000).to_dotted_octet() );
   EXPECT_EQ( "192.168.1.0-192.168.1.255", IP_Range( octets_to_uint32(192,168,1,0), octets_to_uint32(255,255,255,0)).to_dotted_octet() );
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
