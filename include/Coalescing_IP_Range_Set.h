#ifndef COALESCING_IP_RANGE_SET_H
#define COALESCING_IP_RANGE_SET_H

#include <vector>

#include "IP_Range.h"

class Coalescing_IP_Range_Set
{
   public:

      void add( const IP_Range & range );

      int size() const;

      IP_Range at( int index ) const;

   private:

      std::vector<IP_Range> m_ranges;
};

#endif /* COALESCING_IP_RANGE_SET_H */
