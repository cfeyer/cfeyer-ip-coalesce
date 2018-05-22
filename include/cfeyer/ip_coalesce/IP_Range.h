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

#ifndef IP_RANGE_H
#define IP_RANGE_H

#include <cstdint>
#include <string>
#include <iosfwd>

namespace cfeyer {
namespace ip_coalesce {

class IP_Range
{
   public:

      IP_Range();
      IP_Range( uint32_t subnet_address, uint32_t subnet_mask );

      uint32_t get_start_address() const;
      uint32_t get_end_address() const;

      void from_string( const std::string & str );
      bool from_four_octet_address_slash_four_octet_netmask_string( const std::string & str );
      bool from_four_octet_address_slash_netmask_length_string( const std::string & str );
      bool from_four_octet_address_no_netmask_string( const std::string & str );
      bool from_four_octet_address_dash_four_octet_address_string( const std::string & str );

      std::string to_string() const;

      bool is_coalescable( const IP_Range & other ) const;

      bool is_subnet() const;

      uint64_t size() const;

      bool operator == ( const IP_Range & other ) const;
      bool operator < ( const IP_Range & rhs ) const;

      IP_Range & operator += ( const IP_Range & other );

   private:

      friend IP_Range operator + ( const IP_Range & a, const IP_Range & b );

      uint32_t m_start_address;
      uint32_t m_end_address;

      static constexpr uint32_t contiguous_subnet_mask = 0;
      uint32_t m_noncontiguous_subnet_mask;

      std::string to_start_dash_end() const;
      std::string to_start_slash_subnet_mask() const;
      std::string to_cidr() const;
};

std::istream & operator >> ( std::istream & strm, IP_Range & range );
std::ostream & operator << ( std::ostream & strm, const IP_Range & range );

} // namespace ip_coalesce
} // namespace cfeyer

#endif /* IP_RANGE_H */
