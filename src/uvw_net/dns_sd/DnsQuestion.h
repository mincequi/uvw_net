#pragma once

#include <uvw_net/dns_sd/DnsRecordClass.h>
#include <uvw_net/dns_sd/DnsRecordType.h>

namespace uvw_net {
namespace dns_sd {

// +--+--+--+
// / QNAME  /
// +--+--+--+
// | QTYPE  |
// +--+--+--+
// | QCLASS |
// +--+--+--+

struct DnsQuestion {
	std::string name;
	DnsRecordType type = DnsRecordType::Invalid;
	DnsRecordClass cls = DnsRecordClass::Internet;

    const static uint16_t minSize = 6;
};

} // namespace dns_sd
} // namespace uvw_net
