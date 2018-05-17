#include "Coalescing_IP_Range_Set.h"

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
