#include "IP_Range.h"

#include "Format.h"
#include <sstream>

IP_Range::IP_Range( uint32_t subnet_address, uint32_t subnet_mask ) :
   m_start_address( 0 ),
   m_end_address( 0 )
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
