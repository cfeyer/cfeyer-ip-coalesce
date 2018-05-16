#include "Subnet.h"


uint32_t subnet_begin_address( uint32_t subnet_address, uint32_t subnet_mask )
{
   return subnet_address & subnet_mask;
}


uint32_t subnet_end_address( uint32_t subnet_address, uint32_t subnet_mask )
{
   return subnet_address | ~subnet_mask;
}

int count_contiguous_subnet_ones( uint32_t subnet_mask )
{
   int ones = 0;
   int zeros = 0;

   uint32_t buffer = subnet_mask;
   const uint32_t subnet_msb_mask = 0x80000000;

   for( int i = 0; i < 32; i++ )
   {
      uint32_t msb = (buffer & subnet_msb_mask) >> 31;
      buffer = buffer << 1;

      if( msb != 0 )
      {
         ones++;

         if( zeros > 0 )
         {
            return noncontiguous_subnet_mask;
         }
      }
      else
      {
         zeros++;
      }
   }

   return ones; 
}

