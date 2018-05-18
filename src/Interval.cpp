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

#include "Interval.h"

#include <stdexcept>

namespace cfeyer {
namespace ip_coalesce {

bool is_on_or_adjacent( uint32_t x, uint32_t a, uint32_t b )
{
   if( b < a ) throw std::logic_error("Interval end is less than interval start.");

   uint32_t min_on_or_adjacent = (a > 0 ? (a-1) : 0 );
   uint32_t max_on_or_adjacent = (b < 0xffffffff ? (b+1) : 0xffffffff);

   return (min_on_or_adjacent <= x) && (x <= max_on_or_adjacent);
}

}
}
