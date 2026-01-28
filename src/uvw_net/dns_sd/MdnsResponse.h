#pragma once

#include <uvw_net/dns_sd/DnsResourceRecordData.h>

namespace uvw_net {
namespace dns_sd {

struct MdnsResponse {
    std::string serviceName;
    DnsResourceRecordDataPtr ptrData;
    std::optional<DnsResourceRecordDataA> aData;
    std::optional<DnsResourceRecordDataSrv> srvData;
    std::optional<DnsResourceRecordDataTxt> txtData;
};

} // namespace dns_sd
} // namespace uvw_net
