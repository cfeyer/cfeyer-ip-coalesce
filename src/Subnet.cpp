#include "Subnet.h"


uint32_t subnet_start_address( uint32_t subnet_address, uint32_t subnet_mask )
{
   return subnet_address;
}


uint32_t subnet_end_address( uint32_t subnet_address, uint32_t subnet_mask )
{
   int end_address = subnet_address;
   int network_bits = count_contiguous_network_ones( subnet_mask );

   if( network_bits != noncontiguous_subnet_mask )
   {
      uint64_t addresses_in_subnet = netmask_length_to_address_count( network_bits );
      uint64_t highest_address_offset = addresses_in_subnet - 1;
      end_address = subnet_address + static_cast<uint32_t>(highest_address_offset);
   }

   return end_address;
}

int count_contiguous_network_ones( uint32_t subnet_mask )
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


uint64_t netmask_length_to_address_count( int netmask_length_bits )
{
   int host_bits = 32 - netmask_length_bits;
   uint64_t address_count = 1ul << host_bits;
   return address_count;
}

