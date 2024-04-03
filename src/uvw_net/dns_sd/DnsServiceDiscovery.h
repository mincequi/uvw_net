#pragma once

#include <uvw/loop.h>
#include <uvw/udp.h>
#include <uvw_net/dns_sd/DnsRecordDataSrv.h>

namespace uvw_net {
namespace dns_sd {

class DnsServiceDiscovery final: public uvw::emitter<DnsServiceDiscovery, DnsRecordDataSrv> {
public:
    DnsServiceDiscovery();

    void discover(const std::string& name);

private:
    std::shared_ptr<uvw::udp_handle> _udp = uvw::loop::get_default()->resource<uvw::udp_handle>();
    std::string _name;
};

} // namespace dns_sd
} // namespace uvw_net
