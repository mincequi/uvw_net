#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <uvw_net/modbus/ModbusException.h>
#include <uvw_net/modbus/ModbusFunctionCode.h>

namespace uvw_net {
namespace modbus {

class ModbusResponse {
public:
    ModbusResponse();

    static std::optional<ModbusResponse> fromBuffer(const uint8_t buffer[], int length);
    std::vector<uint8_t> toBuffer() const;

    uint16_t transactionId;
    uint16_t protocolId;
    uint8_t unitId;
    ModbusFunctionCode functionCode = ModbusFunctionCode::ReadHoldingRegisters;
    ModbusExceptionCode exceptionCode = ModbusExceptionCode::Illegal;

    uint16_t userData = 0;
    std::vector<std::uint16_t> values;
};

} // namespace modbus
} // namespace uvw_net
