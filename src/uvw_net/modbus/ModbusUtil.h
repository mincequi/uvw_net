#pragma once

#include <cstdint>
#include <vector>

#include <uvw_net/modbus/ModbusException.h>

namespace uvw_net {
namespace modbus {

class ModbusUtil {
public:
    static std::vector<char> createModbusExceptionBuffer(
        uint16_t transactionId,
        uint16_t protocolId,
        uint16_t length,
        uint8_t unitId,
        uint8_t functionCode,
        ModbusExceptionCode exceptionCode);
};

} // namespace modbus
} // namespace uvw_net
