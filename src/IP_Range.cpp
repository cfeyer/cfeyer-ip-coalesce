#include "IP_Range.h"

#include <sstream>

#include "Format.h"
#include "Subnet.h"

IP_Range::IP_Range( uint32_t subnet_address, uint32_t subnet_mask ) :
   m_start_address( subnet_start_address(subnet_address, subnet_mask) ),
   m_end_address( subnet_end_address(subnet_address, subnet_mask) )
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
