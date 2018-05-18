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

#ifndef COALESCING_IP_RANGE_SET_H
#define COALESCING_IP_RANGE_SET_H

#include <set>

#include <cfeyer/ip_coalesce/IP_Range.h>

namespace cfeyer {
namespace ip_coalesce {

using IP_Range_Set = std::set<IP_Range>;

class Coalescing_IP_Range_Set
{
   public:

      void insert( const IP_Range & range );

      int size() const;

      IP_Range_Set::const_iterator begin() const;
      IP_Range_Set::const_iterator end() const;

   private:

      IP_Range_Set m_ranges;
};

} // namespace ip_coalesce
} // namespace cfeyer

#endif /* COALESCING_IP_RANGE_SET_H */
