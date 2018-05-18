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

#include <cfeyer/ip_coalesce/IP_Range.h>

#include <sstream>
#include <algorithm>
#include <vector>
#include <stdexcept>

#include "Format.h"
#include "Subnet.h"
#include "Interval.h"

namespace cfeyer {
namespace ip_coalesce {

IP_Range::IP_Range( uint32_t subnet_address, uint32_t subnet_mask ) :
   m_start_address( subnet_start_address(subnet_address, subnet_mask) ),
   m_end_address( subnet_end_address(subnet_address, subnet_mask) ),
   m_noncontiguous_subnet_mask(
      (count_contiguous_network_ones(subnet_mask) == noncontiguous_subnet_mask) ?
         subnet_mask : contiguous_subnet_mask
   )
{
   if( m_end_address < m_start_address )
   {
      std::ostringstream msg;
      msg << "IP_Range: m_end_address < m_start_address "
          << "(m_start_address=" << m_start_address << ", "
          << "m_end_address=" << m_end_address << ") "
          << "(network too large for its start address?)";
      throw std::logic_error( msg.str() );
   }
}

IP_Range::IP_Range() :
   m_start_address(0),
   m_end_address(0),
   m_noncontiguous_subnet_mask( contiguous_subnet_mask )
{
}


uint32_t IP_Range::get_start_address() const
{
   return m_start_address;
}


uint32_t IP_Range::get_end_address() const
{
   return m_end_address;
}


std::string IP_Range::to_string() const
{
   std::string str;

   if( !m_noncontiguous_subnet_mask )
   {
      if( (size() > 1) && is_subnet() )
      {
         str = to_cidr();
      }
      else
      {
         str = to_start_dash_end();
      }
   }
   else
   {
      str = to_start_slash_subnet_mask();
   }

   return str;
}


std::string IP_Range::to_start_dash_end() const
{
   std::string str;

   if( m_start_address == m_end_address )
   {
      str = to_dotted_octet( m_start_address );
   }
   else
   {
      std::ostringstream strm;
      strm << to_dotted_octet( m_start_address )
           << "-"
           << to_dotted_octet( m_end_address );
      str = strm.str();
   }

   return str;
}


std::string IP_Range::to_start_slash_subnet_mask() const
{
   if( !m_noncontiguous_subnet_mask ) throw std::logic_error("Subnet mask not available");

   std::ostringstream strm;

   strm << to_dotted_octet( m_start_address ) << "/"
        << to_dotted_octet( m_noncontiguous_subnet_mask );

   return strm.str();
}


std::string IP_Range::to_cidr() const
{
   std::ostringstream strm;

   strm << to_dotted_octet( m_start_address ) << "/"
        << (32 - ::cfeyer::ip_coalesce::log_base_2(size()) );

   return strm.str();
}


bool IP_Range::is_coalescable( const IP_Range & other ) const
{
   bool predicate = false;

   if( !m_noncontiguous_subnet_mask && !other.m_noncontiguous_subnet_mask )
   {
      predicate =
         is_on_or_adjacent( other.m_start_address, m_start_address, m_end_address ) ||
         is_on_or_adjacent( other.m_end_address, m_start_address, m_end_address );
   }

   return predicate;
}


bool IP_Range::is_subnet() const
{
   return (m_noncontiguous_subnet_mask == contiguous_subnet_mask) &&
          ::cfeyer::ip_coalesce::is_subnet( m_start_address, size() );
}


uint64_t IP_Range::size() const
{
   return (static_cast<uint64_t>(m_end_address) - static_cast<uint64_t>(m_start_address)) + 1;
}


bool IP_Range::operator == ( const IP_Range & other ) const
{
   return (m_start_address == other.m_start_address) &&
          (m_end_address == other.m_end_address);
}


bool IP_Range::operator < ( const IP_Range & rhs ) const
{
   if( m_start_address != rhs.m_start_address )
   {
      return (m_start_address < rhs.m_start_address);
   }
   else
   {
      return (m_end_address < rhs.m_end_address );
   }
}


IP_Range operator + ( const IP_Range & a, const IP_Range & b ) {

   if( !a.is_coalescable(b) ) throw std::runtime_error("Ranges not coalescable");

   IP_Range result;

   result.m_start_address = std::min( a.m_start_address, b.m_start_address );
   result.m_end_address = std::max( a.m_end_address, b.m_end_address );

   return result;
}


IP_Range & IP_Range::operator += ( const IP_Range & other )
{
   *this = *this + other;
   return *this;
}


std::istream & operator >> ( std::istream & istrm, IP_Range & range )
{
   std::vector<std::string> substrings(8);

   if(
      std::getline( istrm, substrings[0], '.' ) &&
      std::getline( istrm, substrings[1], '.' ) &&
      std::getline( istrm, substrings[2], '.' ) &&
      std::getline( istrm, substrings[3], '/' ) &&
      std::getline( istrm, substrings[4], '.' ) &&
      std::getline( istrm, substrings[5], '.' ) &&
      std::getline( istrm, substrings[6], '.' ) &&
      istrm >> substrings[7]
   )
   {
      std::vector<uint8_t> octets(8);
      for( int i = 0; i < 8; i++ )
      {
         octets[i] = std::stoi( substrings[i] );
      }

      range = IP_Range( from_octets(octets[0], octets[1], octets[2], octets[3]),
                        from_octets(octets[4], octets[5], octets[6], octets[7]) );
   }

   return istrm;
}


std::ostream & operator << ( std::ostream & ostrm, const IP_Range & range )
{
   ostrm << range.to_string();
   return ostrm;
}

} // namespace ip_coalesce
} // namespace cfeyer
