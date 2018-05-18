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
