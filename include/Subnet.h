#ifndef SUBNET_H
#define SUBNET_H

#include <cstdint>

uint32_t subnet_begin_address( uint32_t subnet_address, uint32_t subnet_mask );
uint32_t subnet_end_address( uint32_t subnet_address, uint32_t subnet_mask );

constexpr int noncontiguous_subnet_mask = -1;

int count_contiguous_subnet_ones( uint32_t subnet_mask );

#endif /* SUBNET_H */
