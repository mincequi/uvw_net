#pragma once

#include <optional>
#include <string>

#include <uvw_net/dns_sd/DnsLabels.h>

namespace uvw_net {
namespace dns_sd {

struct DnsResourceRecordDataSrv {
    uint16_t priority;
    uint16_t weight;
    uint16_t port;
    std::string target;

    static std::optional<DnsResourceRecordDataSrv> fromBuffer(const uint8_t* head, /*const DnsLabels& labels,*/ const uint8_t* data, size_t size);
};

} // namespace dns_sd
} // namespace uvw_net
