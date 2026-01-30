#pragma once

#include <uvw_net/dns_sd/DnsResourceRecordClass.h>
#include <uvw_net/dns_sd/DnsResourceRecordData.h>
#include <uvw_net/dns_sd/DnsResourceRecordType.h>

namespace uvw_net {
namespace dns_sd {

// +--+--+--+--
// /   NAME   /
// +--+--+--+--
// |   TYPE   |
// +--+--+--+--
// |   CLASS  |
// +--+--+--+--
// |   TTL    |
// +--+--+--+--
// | RDLENGTH |
// +--+--+--+--
// /   RDATA  /
// +--+--+--+--

struct DnsResourceRecord {
	std::string name;
    // note: type is inheritly defined by data variant, therefore not needed here
    //DnsRecordType type = DnsRecordType::Invalid;
    // note: class is always IN for DNS-SD, therefore not needed here
    //DnsResourceRecordClass cls = DnsResourceRecordClass::Invalid;
	uint32_t ttl = 0;
    DnsResourceRecordData data;

    const static uint16_t minSize = 6;
};

} // namespace dns_sd
} // namespace uvw_net
