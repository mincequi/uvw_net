#include "ModbusDiscovery.h"

#include <iostream>

#include <uvw/util.h>

namespace uvw_net {
namespace modbus {

ModbusDiscovery::ModbusDiscovery() {
}

void ModbusDiscovery::discover() {
    _candidates.clear();

    // Find subnet to scan
    std::string subnet;
    const auto interfaces = uvw::utilities::interface_addresses();
    for (const auto& interface : interfaces) {
        if (interface.netmask.ip != "255.255.255.0") continue;

        const auto ip = interface.address.ip;
        subnet = ip.substr(0, ip.find_last_of(".")+1);
        break;
    }
    if (subnet.empty()) {
        std::cerr << "no interface with subnet mask 255.255.255.0 found" << std::endl;
        return;
    }

    // Scan subnets
    for (uint8_t i = 1; i < 255; ++i) {
        const std::string host = subnet + std::to_string(i);

        auto candidate = ModbusClient::create();
        candidate->on<uvw::connect_event>([this](const uvw::connect_event&, ModbusClient& thing) {
            thing.reset();
            publish(thing.shared_from_this());
        });
        candidate->on<uvw::error_event>([this](const uvw::error_event&, ModbusClient& thing){
            thing.reset();
        });

        candidate->connect(host);
        _candidates.push_back(std::move(candidate));
    }
}

} // namespace modbus
} // namespace uvw_net
