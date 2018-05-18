#ifndef SUBNET_H
#define SUBNET_H

#include <cstdint>

namespace cfeyer {
namespace ip_coalesce {

uint32_t subnet_start_address( uint32_t subnet_address, uint32_t subnet_mask );
uint32_t subnet_end_address( uint32_t subnet_address, uint32_t subnet_mask );

constexpr int noncontiguous_subnet_mask = -1;
int count_contiguous_network_ones( uint32_t subnet_mask );

uint64_t netmask_length_to_address_count( int netmask_length_bits );

bool is_subnet( uint32_t address, uint64_t size );

bool is_power_of_2( uint64_t x );
int log_base_2( uint64_t x );

uint32_t size_to_subnet_mask( uint64_t size );


} // namespace ip_coalesce
} // namespace cfeyer

#endif /* SUBNET_H */
