#pragma once

#include <cstdint>

namespace uvw_net {
namespace dns_sd {

enum class DnsOpcode : uint8_t {
    Query = 0,
};

}
}
