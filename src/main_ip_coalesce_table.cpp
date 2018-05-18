#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include <cfeyer/ip-coalesce/IP_Range.h>
#include <cfeyer/ip-coalesce/Coalescing_IP_Range_Set.h>

void process_field_2( const std::string & field_2 )
{
   std::istringstream f2_strm( field_2 );
   Coalescing_IP_Range_Set set;
   std::string range_str;

   static constexpr char item_delim = ',';

   while( std::getline( f2_strm, range_str, item_delim ) )
   {
      std::istringstream range_strm( range_str );
      IP_Range range;
      range_strm >> range;
      set.insert( range );
   }

   bool needs_preceeding_delimiter = false;
   for( auto range : set )
   {
      if( needs_preceeding_delimiter )
      {
         std::cout << item_delim;
      }
      std::cout << range;
      needs_preceeding_delimiter = true;
   }
}

void process_line( const std::string & line )
{
   std::istringstream line_strm( line );

   static constexpr char field_delim = ':';

   std::string field_1;
   if( std::getline( line_strm, field_1, field_delim ) )
   {
      std::cout << field_1 << field_delim;
   }
   else
   {
      throw std::runtime_error( "Error parsing field 1" );
   }
   
   std::string field_2;
   if( line_strm >> field_2 )
   {
      process_field_2( field_2 );
   }
   else
   {
      throw std::runtime_error( "Error parsing field 2" );
   }

   std::cout << '\n';
}

int main( int argc, char * argv[] )
{
   std::string line;

   while( std::getline( std::cin, line ) )
   {
      process_line( line );
   }

   std::cout.flush();

   return 0;
}

