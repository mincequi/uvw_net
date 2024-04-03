#include <iostream>

#include <magic_enum_iostream.hpp>

#include <uvw/timer.h>
#include <uvw_net/dns_sd/DnsServiceDiscovery.h>
#include <uvw_net/modbus/ModbusDiscovery.h>
#include <uvw_net/sunspec/SunSpecDiscovery.h>

using namespace uvw_net::dns_sd;
using namespace uvw_net::modbus;
using namespace uvw_net::sunspec;

using magic_enum::iostream_operators::operator<<;

int main() {
    DnsServiceDiscovery dnsDiscovery;
    dnsDiscovery.discover("_http._tcp.local");
    dnsDiscovery.on<DnsRecordDataSrv>([](const DnsRecordDataSrv& data, const DnsServiceDiscovery&) {
        const auto host = data.target.substr(0, data.target.find("."));
        std::cout << "http service found> host: " << host << ", port: " << data.port << std::endl;
    });

    SunSpecDiscovery sunspecDiscovery;
    sunspecDiscovery.on<SunSpecClientPtr>([](SunSpecClientPtr thing, const SunSpecDiscovery&) {
        std::stringstream supportedModels;
        std::stringstream unsupportedModels;
        for (const auto& kv : thing->models()) {
            auto m = magic_enum::enum_cast<SunSpecModel::Id>(kv.first);
            if (m.has_value())
                supportedModels << magic_enum::enum_name(m.value())  << " (" << kv.first << "), ";
            //else
            //    unsupportedModels << kv.first << ", ";
        }
        std::cout << "sunspec thing found> id: " << thing->sunSpecId()
                  << ", host: " << thing->host()
                  << ", unitId: " << (uint32_t)thing->unitId()
                  << ", supportedModels: " << supportedModels.str() << std::endl;
                  //<< "unsupportedModels: " << unsupportedModels.str();
    });

    ModbusDiscovery modbusDiscovery;
    // ModbusDiscovery spawns ready/connected ModbusThings
    modbusDiscovery.on<ModbusClientPtr>([&](ModbusClientPtr thing, const ModbusDiscovery&) {
        std::cout << "modbus thing found> ip: " << thing->ip() << std::endl;
        sunspecDiscovery.discover(thing);
    });

    // Start a timeout
    auto timer = uvw::loop::get_default()->resource<uvw::timer_handle>();
    timer->on<uvw::timer_event>([&](const auto&, auto&) {
        modbusDiscovery.discover();
    });
    timer->start(uvw::timer_handle::time{0}, uvw::timer_handle::time{30000});

    // Run the event loop
    return uvw::loop::get_default()->run();
}
