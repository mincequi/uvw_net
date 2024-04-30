#pragma once

#include <uvw_net/modbus/ModbusClient.h>

namespace uvw_net {
namespace modbus {

class ModbusDiscovery final: public uvw::emitter<ModbusDiscovery, ModbusClientPtr> {
public:
    ModbusDiscovery();

    void discover();

private:
    std::map<std::string, ModbusClientPtr> _candidates;
};

} // namespace modbus
} // namespace uvw_net
