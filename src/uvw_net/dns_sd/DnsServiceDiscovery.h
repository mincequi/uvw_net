#pragma once

#include <uvw/loop.h>
#include <uvw/udp.h>
#include <uvw_net/dns_sd/DnsRecordDataSrv.h>
#include <uvw_net/dns_sd/MdnsResponse.h>

namespace uvw_net {
namespace dns_sd {

class DnsServiceDiscovery final: public uvw::emitter<DnsServiceDiscovery, MdnsResponse> {
public:
    DnsServiceDiscovery(const std::string& name);

    void discover();

private:
    std::shared_ptr<uvw::udp_handle> _udp = uvw::loop::get_default()->resource<uvw::udp_handle>();
    std::string _name;
};

} // namespace dns_sd
} // namespace uvw_net
