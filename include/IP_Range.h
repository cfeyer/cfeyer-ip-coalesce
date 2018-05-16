#ifndef IP_RANGE_H
#define IP_RANGE_H

#include <cstdint>
#include <string>

class IP_Range
{
   public:

      IP_Range( uint32_t subnet_address, uint32_t subnet_mask );

      uint32_t get_start_address() const;
      uint32_t get_end_address() const;

      std::string to_dotted_octet() const;



   private:

      uint32_t m_start_address;
      uint32_t m_end_address;
};

#endif /* IP_RANGE_H */
