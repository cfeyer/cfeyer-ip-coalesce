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

#include <cfeyer/ip_coalesce/Coalescing_IP_Range_Set.h>

namespace cfeyer {
namespace ip_coalesce {

void Coalescing_IP_Range_Set::insert( const IP_Range & range )
{
   bool incorporated_via_coalescing = false;

   for( auto iter = m_ranges.begin();
        iter != m_ranges.end();
        iter++ )
   {
      const IP_Range & existing_range = *iter;

      if( existing_range.is_coalescable( range ) )
      {
         IP_Range coalesced_range = existing_range + range;
         m_ranges.erase( iter );
         m_ranges.insert( coalesced_range );

         incorporated_via_coalescing = true;

         Coalescing_IP_Range_Set recoalesced_set;
         for( const IP_Range & range : m_ranges )
         {
            recoalesced_set.insert( range );
         }
         *this = recoalesced_set;

         break;
      }
   }

   if( !incorporated_via_coalescing )
   {
      m_ranges.insert( range );
   }
}


IP_Range_Set::const_iterator Coalescing_IP_Range_Set::begin() const
{
   return m_ranges.cbegin();
}


IP_Range_Set::const_iterator Coalescing_IP_Range_Set::end() const
{
   return m_ranges.cend();
}


int Coalescing_IP_Range_Set::size() const
{
  return m_ranges.size();
}

} // namespace ip_coalesce
} // namespace cfeyer
