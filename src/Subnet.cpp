#include "Subnet.h"

#include <stdexcept>

namespace cfeyer {
namespace ip_coalesce {


uint32_t subnet_start_address( uint32_t subnet_address, uint32_t subnet_mask )
{
   int start_address = subnet_address;
   int network_bits = count_contiguous_network_ones( subnet_mask );

   if( network_bits != noncontiguous_subnet_mask )
   {
      start_address = subnet_address & subnet_mask;
   }

   return start_address;
}


uint32_t subnet_end_address( uint32_t subnet_address, uint32_t subnet_mask )
{
   int end_address = subnet_address;
   int network_bits = count_contiguous_network_ones( subnet_mask );

   if( network_bits != noncontiguous_subnet_mask )
   {
      uint64_t addresses_in_subnet = netmask_length_to_address_count( network_bits );
      uint64_t highest_address_offset = addresses_in_subnet - 1;
      uint32_t start_address = subnet_start_address( subnet_address, subnet_mask );
      end_address = start_address + static_cast<uint32_t>(highest_address_offset);
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


bool is_subnet( uint32_t address, uint64_t size )
{
   bool predicate = false;

   if( is_power_of_2(size) )
   {
      uint32_t netmask = size_to_subnet_mask( size );
      predicate = (address == (address & netmask));
   }

   return predicate;
}


bool is_power_of_2( uint64_t x )
{
   int ones = 0;

   for( int i = 0; i < 64; i++ )
   {
      ones += (x & 0x1);
      x >>= 1;
   }

   return (ones == 1);
}


int log_base_2( uint64_t x )
{
   if( !is_power_of_2(x) ) throw std::domain_error( "Argument to log_base_2() must be power of two." );

   int zeros = 0;

   for( zeros = 0; zeros < 64; zeros++ )
   {
      if( (x & 0x1) == 1 ) break;
      x >>= 1;
   }

   return zeros;
}


uint32_t size_to_subnet_mask( uint64_t size )
{
   if( size > 0x100000000 ) throw std::domain_error( "Size of subnet cannot exceed 2^32." );

   int zeros = log_base_2(size);

   uint32_t mask = 0xffffffffULL << zeros;

   return mask;
}

} // namespace ip_coalesce
} // namespace cfeyer
