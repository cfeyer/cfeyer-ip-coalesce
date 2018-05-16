#include "gtest/gtest.h"

#include "IP_Range.h"
#include "Format.h"
#include "Subnet.h"

TEST(Format, test_to_dotted_octet) {
   EXPECT_EQ( std::string("0.0.0.0"), to_dotted_octet(0x00000000) );
   EXPECT_EQ( std::string("255.255.255.255"), to_dotted_octet(0xffffffff) );
   EXPECT_EQ( std::string("1.2.3.4"), to_dotted_octet(0x01020304) );
}

TEST(Format, test_octets_to_uint32) {
   EXPECT_EQ( 0x00000000, octets_to_uint32(0,0,0,0) );
   EXPECT_EQ( 0x01020304, octets_to_uint32(1,2,3,4) );
   EXPECT_EQ( 0xFF0000FF, octets_to_uint32(255,0,0,255) );
}

TEST(Subnet, test_count_contiguous_subnet_ones) {
   EXPECT_EQ( 0, count_contiguous_subnet_ones(0x00000000) );
   EXPECT_EQ( 24, count_contiguous_subnet_ones(0xffffff00) );
   EXPECT_EQ( 32, count_contiguous_subnet_ones(0xffffffff) );
   EXPECT_EQ( 32, count_contiguous_subnet_ones(0xffffffff) );
   EXPECT_EQ( noncontiguous_subnet_mask, count_contiguous_subnet_ones(0xff0000ff) );
}


//TEST(Subnet, test_end_address) {
//   {
//      uint32_t expected = 0x00000000;
//      uint32_t actual = subnet_end_address( 0x00000000, 0x00000000 );
//      EXPECT_EQ( expected, actual );
//   }
//
//   {
//      uint32_t expected = 0xffffffff;
//      uint32_t actual = subnet_end_address( 0x00000000, 0xffffffff );
//      EXPECT_EQ( expected, actual );
//   }
//}
//
//TEST(IP_Range, test_to_dotted_octet) {
//   EXPECT_EQ( std::string("0.0.0.0"), IP_Range(0x00000000, 0x00000000).to_dotted_octet() );
//   EXPECT_EQ( std::string("0.0.0.0-255.255.255.0"), IP_Range(0x00000000, 0xffffffff).to_dotted_octet() );
//}
//
