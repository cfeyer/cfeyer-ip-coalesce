#ifndef IP_RANGE_H
#define IP_RANGE_H

#include <cstdint>
#include <string>
#include <iosfwd>

namespace cfeyer {
namespace ip_coalesce {

class IP_Range
{
   public:

      IP_Range();
      IP_Range( uint32_t subnet_address, uint32_t subnet_mask );

      uint32_t get_start_address() const;
      uint32_t get_end_address() const;

      std::string to_string() const;

      bool is_coalescable( const IP_Range & other ) const;

      bool is_subnet() const;

      uint64_t size() const;

      bool operator == ( const IP_Range & other ) const;
      bool operator < ( const IP_Range & rhs ) const;

      IP_Range & operator += ( const IP_Range & other );

   private:

      friend IP_Range operator + ( const IP_Range & a, const IP_Range & b );

      uint32_t m_start_address;
      uint32_t m_end_address;

      static constexpr uint32_t contiguous_subnet_mask = 0;
      uint32_t m_noncontiguous_subnet_mask;

      std::string to_start_dash_end() const;
      std::string to_start_slash_subnet_mask() const;
      std::string to_cidr() const;
};

std::istream & operator >> ( std::istream & strm, IP_Range & range );
std::ostream & operator << ( std::ostream & strm, const IP_Range & range );

} // namespace ip_coalesce
} // namespace cfeyer

#endif /* IP_RANGE_H */
