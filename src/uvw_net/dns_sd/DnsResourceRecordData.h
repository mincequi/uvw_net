#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>

#include <uvw_net/dns_sd/DnsRecordDataSrv.h>
#include <uvw_net/dns_sd/DnsResourceRecordType.h>

namespace uvw_net {
namespace dns_sd {

using DnsResourceRecordDataOctectStream = std::vector<std::byte>;

struct DnsResourceRecordDataA {
    uint32_t ipv4 = 0;
};

using DnsResourceRecordDataAaaa = std::array<std::byte, 16>;

using DnsResourceRecordDataPtr = std::string;

struct DnsResourceRecordDataTxt {
	uint8_t size = 0;
	std::string txt;
};

using DnsResourceRecordData = std::variant<    
    DnsResourceRecordDataOctectStream,
    DnsResourceRecordDataA,
    DnsResourceRecordDataAaaa,
    DnsResourceRecordDataPtr,
    DnsResourceRecordDataTxt,
    DnsResourceRecordDataSrv>;

} // namespace dns_sd
} // namespace uvw_net
