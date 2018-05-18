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
