#ifndef FORMAT_H
#define FORMAT_H

#include <cstdint>
#include <string>

namespace cfeyer {
namespace ip_coalesce {

std::string to_dotted_octet( uint32_t ip_address );

uint32_t from_octets( uint8_t o3, uint8_t o2, uint8_t o1, uint8_t o0 );

}
}

#endif /*FORMAT_H*/
