#pragma once

#include <string>

#include <uvw_net/dns_sd/DnsResourceRecordClass.h>
#include <uvw_net/dns_sd/DnsResourceRecordType.h>

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
    DnsResourceRecordType type = DnsResourceRecordType::PTR;
	DnsResourceRecordClass cls = DnsResourceRecordClass::Internet;

    const static uint16_t minSize = 6;
};

} // namespace dns_sd
} // namespace uvw_net
