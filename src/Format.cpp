#include "Format.h"

#include <sstream>

std::string to_dotted_octet( uint32_t ip_address )
{
   std::ostringstream strm;

   uint32_t x = ip_address;

   for( int i = 0; i < 4; i++ )
   {
      uint32_t high_octet = (x & 0xff000000) >> 24;
      strm << high_octet;
      if( i < 3)
      {
         strm << ".";
      }
      x = x << 8;
   }

   return strm.str();
}


uint32_t from_octets( uint8_t o3, uint8_t o2, uint8_t o1, uint8_t o0 )
{
   uint32_t i = (o3 << 24) | (o2 << 16) | (o1 << 8) | (o0);
   return i;
}
