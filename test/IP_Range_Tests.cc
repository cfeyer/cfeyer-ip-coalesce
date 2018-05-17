#include "gtest/gtest.h"

#include "IP_Range.h"
#include "Format.h"
#include "Subnet.h"

TEST(Format, test_to_dotted_octet) {
   EXPECT_EQ( std::string("0.0.0.0"), to_dotted_octet(0x00000000) );
   EXPECT_EQ( std::string("255.255.255.255"), to_dotted_octet(0xffffffff) );
   EXPECT_EQ( std::string("1.2.3.4"), to_dotted_octet(0x01020304) );
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

TEST(IP_Range, test_to_dotted_octet) {
   EXPECT_EQ( std::string("0.0.0.0"), IP_Range(0x00000000, 0xffffffff).to_dotted_octet() );
   EXPECT_EQ( std::string("0.0.0.0-255.255.255.255"), IP_Range(0x00000000, 0x00000000).to_dotted_octet() );
   EXPECT_EQ( std::string("192.168.1.0-192.168.1.255"), IP_Range( octets_to_uint32(192,168,1,0), octets_to_uint32(255,255,255,0)).to_dotted_octet() );
}

