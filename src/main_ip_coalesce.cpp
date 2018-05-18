#include <iostream>

#include <cfeyer/ip_coalesce/IP_Range.h>
#include <cfeyer/ip_coalesce/Coalescing_IP_Range_Set.h>

using namespace cfeyer::ip_coalesce;

int main( int argc, char * argv[] )
{
   Coalescing_IP_Range_Set set;

   IP_Range range;

   while( std::cin >> range )
   {
      set.insert( range );
   }

   bool needs_preceeding_delimiter = false;
   for( auto range : set )
   {
      std::cout << (needs_preceeding_delimiter ? " " : "")
                << range;
      needs_preceeding_delimiter = true;
   }

   return 0;
}

