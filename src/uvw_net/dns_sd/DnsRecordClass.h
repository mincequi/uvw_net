#pragma once

#include <cstdint>

namespace uvw_net {
namespace dns_sd {

enum DnsResourceRecordClass : uint16_t {
    Invalid = 0,

	Internet = 1,
	CHAOS = 3,
	Hesiod = 4,
};

} // namespace dns_sd
} // namespace uvw_net
