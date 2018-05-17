#include "Coalescing_IP_Range_Set.h"

void Coalescing_IP_Range_Set::add( const IP_Range & range )
{
   bool incorporated_via_coalescing = false;

   for( IP_Range & existing_range : m_ranges )
   {
      if( existing_range.is_coalescable( range ) )
      {
         existing_range += range;
         incorporated_via_coalescing = true;

         Coalescing_IP_Range_Set recoalesced_set;
         for( const IP_Range & range : m_ranges )
         {
            recoalesced_set.add( range );
         }
         *this = recoalesced_set;

         break;
      }
   }

   if( !incorporated_via_coalescing )
   {
      m_ranges.push_back( range );
   }
}

IP_Range Coalescing_IP_Range_Set::at( int index ) const
{
   return m_ranges.at( index );
}

int Coalescing_IP_Range_Set::size() const
{
  return m_ranges.size();
}
