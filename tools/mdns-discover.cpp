#include <iostream>

#include <magic_enum_iostream.hpp>

#include <uvw/timer.h>
#include <uvw_net/dns_sd/DnsServiceDiscovery.h>

using namespace uvw_net::dns_sd;

using magic_enum::iostream_operators::operator<<;

int main() {
    DnsServiceDiscovery dnsDiscovery("_shelly._tcp.local.");
    dnsDiscovery.on<MdnsResponse>([](const MdnsResponse& response, const DnsServiceDiscovery&) {
        std::cout << "shelly found> ptrData: " << response.ptrData << std::endl;
    });

    // Start a timeout
    auto timer = uvw::loop::get_default()->resource<uvw::timer_handle>();
    timer->on<uvw::timer_event>([&](const auto&, auto&) {
        dnsDiscovery.discover();
    });
    timer->start(uvw::timer_handle::time{1000}, uvw::timer_handle::time{5000});

    // Run the event loop
    return uvw::loop::get_default()->run();
}
