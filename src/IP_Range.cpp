#include "IP_Range.h"

#include <sstream>
#include <algorithm>
#include <vector>
#include <stdexcept>

#include "Format.h"
#include "Subnet.h"
#include "Interval.h"

IP_Range::IP_Range( uint32_t subnet_address, uint32_t subnet_mask ) :
   m_start_address( subnet_start_address(subnet_address, subnet_mask) ),
   m_end_address( subnet_end_address(subnet_address, subnet_mask) )
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
   m_end_address(0)
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


std::string IP_Range::to_dotted_octet() const
{
   std::string str;

   if( m_start_address == m_end_address )
   {
      str = ::to_dotted_octet( m_start_address );
   }
   else
   {
      std::ostringstream strm;
      strm << ::to_dotted_octet( m_start_address )
           << "-"
           << ::to_dotted_octet( m_end_address );
      str = strm.str();
   }

   return str;
}


bool IP_Range::is_coalescable( const IP_Range & other ) const
{
   return is_on_or_adjacent( other.m_start_address, m_start_address, m_end_address ) ||
          is_on_or_adjacent( other.m_end_address, m_start_address, m_end_address );
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
   ostrm << range.to_dotted_octet();
   return ostrm;
}
