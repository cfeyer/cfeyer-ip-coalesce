#ifndef COALESCING_IP_RANGE_SET_H
#define COALESCING_IP_RANGE_SET_H

#include <set>

#include <cfeyer/ip-coalesce/IP_Range.h>

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

#endif /* COALESCING_IP_RANGE_SET_H */
