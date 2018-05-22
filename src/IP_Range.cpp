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
#include <regex>
#include <string>
#include <iostream> //TODO debug

#include "Format.h"
#include "CIDR_Network.h"
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


void IP_Range::from_string( const std::string & str )
{
   bool success =
      from_four_octet_address_slash_four_octet_netmask_string( str ) ||
      from_four_octet_address_slash_netmask_length_string( str ) ||
      from_four_octet_address_dash_four_octet_address_string( str ) ||
      from_four_octet_address_no_netmask_string( str );

   if( !success )
   {
      std::ostringstream msg;
      msg << "Failed to parse '" << str << "'.";
      throw std::runtime_error( msg.str() );
   }
}


bool IP_Range:: from_four_octet_address_slash_four_octet_netmask_string( const std::string & str )
{
   bool success = false;

   std::smatch matches;
   static const std::regex regex( "([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)/([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)" );

   if( std::regex_match( str, matches, regex ) )
   {
      std::vector<std::uint8_t> octets(8);

      for( int i = 1; i < matches.size(); i++ )
      {
         const std::string & octet_str = matches[i];
         uint8_t octet = std::stoi( octet_str );
         octets.at(i-1) = octet;
      }

      *this = IP_Range( from_octets(octets.at(0), octets.at(1), octets.at(2), octets.at(3) ),
                        from_octets(octets.at(4), octets.at(5), octets.at(6), octets.at(7) ) );

      success = true;
   }

   return success;
}


bool IP_Range:: from_four_octet_address_slash_netmask_length_string( const std::string & str )
{
   bool success = false;

   std::smatch matches;
   static const std::regex regex( "([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)/([0-9]+)" );

   if( std::regex_match( str, matches, regex ) )
   {
      std::vector<std::uint8_t> octets(4);

      for( int i = 1; i < (matches.size()-1); i++ )
      {
         const std::string & octet_str = matches[i];
         uint8_t octet = std::stoi( octet_str );
         octets.at(i-1) = octet;
      }

      int netmask_length = std::stoi( matches[matches.size()-1] );

      *this = IP_Range( from_octets(octets.at(0), octets.at(1), octets.at(2), octets.at(3) ),
                        size_to_subnet_mask( netmask_length_to_address_count( netmask_length ) ) );

      success = true;
   }

   return success;
}


bool IP_Range:: from_four_octet_address_no_netmask_string( const std::string & str )
{
   bool success = false;

   std::smatch matches;
   static const std::regex regex( "([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)" );

   if( std::regex_match( str, matches, regex ) )
   {
      std::vector<std::uint8_t> octets(4);

      for( int i = 1; i < matches.size(); i++ )
      {
         const std::string & octet_str = matches[i];
         uint8_t octet = std::stoi( octet_str );
         octets.at(i-1) = octet;
      }

      *this = IP_Range( from_octets(octets.at(0), octets.at(1), octets.at(2), octets.at(3) ),
                        0xffffffff );

       success = true;
   }

   return success;
}


bool IP_Range:: from_four_octet_address_dash_four_octet_address_string( const std::string & str )
{
   bool success = false;

   std::smatch matches;
   static const std::regex regex( "([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)-([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)" );

   if( std::regex_match( str, matches, regex ) )
   {
      std::vector<std::uint8_t> octets(8);

      for( int i = 1; i < matches.size(); i++ )
      {
         const std::string & octet_str = matches[i];
         uint8_t octet = std::stoi( octet_str );
         octets.at(i-1) = octet;
      }

      *this = IP_Range();
      m_start_address = from_octets( octets.at(0), octets.at(1), octets.at(2), octets.at(3) );
      m_end_address = from_octets( octets.at(4), octets.at(5), octets.at(6), octets.at(7) );

      success = true;
   }

   return success;
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
   std::string str;
   istrm >> str;

   if( !str.empty() )
   {
      range.from_string( str );
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
