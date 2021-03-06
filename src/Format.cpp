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

#include "Format.h"

#include <sstream>
#include <vector>

namespace cfeyer {
namespace ip_coalesce {

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

} // namespace ip_coalesce
} // namespace cfeyer
