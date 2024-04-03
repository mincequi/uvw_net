#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <uvw_net/modbus/ModbusException.h>

namespace uvw_net {
namespace modbus {

class ModbusResponse {
public:
    static std::optional<ModbusResponse> fromBuffer(const uint8_t buffer[], int length);

    uint16_t transactionId;
    uint16_t protocolId;
    uint8_t unitId;
    uint8_t functionCode;
    ModbusExceptionCode exceptionCode = ModbusExceptionCode::Illegal;

    uint16_t userData = 0;
    std::vector<std::uint16_t> values;

private:
    ModbusResponse();
};

} // namespace modbus
} // namespace uvw_net
