#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>

#include <uvw_net/dns_sd/DnsRecordDataSrv.h>
#include <uvw_net/dns_sd/DnsRecordType.h>

namespace uvw_net {
namespace dns_sd {

using DnsResourceRecordDataOctectStream = std::vector<std::byte>;

using DnsResourceRecordDataA = uint32_t;

using DnsResourceRecordDataAaaa = std::array<std::byte, 16>;

using DnsResourceRecordDataPtr = std::string;

struct MXData {
	uint16_t preference = 0;
	std::string exchange;
};

struct DnsResourceRecordDataTxt {
	uint8_t size = 0;
	std::string txt;
};

struct SOAData {
	std::string primaryServer;
	std::string administrator;
	uint32_t serialNo = 0;
	uint32_t refresh = 0;
	uint32_t retry = 0;
	uint32_t expire = 0;
	uint32_t defaultTtl = 0;
};

using DnsResourceRecordData = std::variant<
        DnsResourceRecordDataOctectStream,
        DnsResourceRecordDataA,
		//AAAAData,
        DnsResourceRecordDataPtr,
		MXData,
        DnsResourceRecordDataTxt,
		SOAData,
        DnsResourceRecordDataSrv>;

} // namespace dns_sd
} // namespace uvw_net
