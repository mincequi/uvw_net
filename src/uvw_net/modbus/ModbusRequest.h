#pragma once

#include <array>
#include <cstdint>

#include <uvw_net/modbus/ModbusFunctionCode.h>

namespace uvw_net {
namespace modbus {

class ModbusRequest {
public:
    ModbusRequest();

    uint16_t transactionId = 0x00;
    uint8_t unitId;
    ModbusFunctionCode functionCode = ModbusFunctionCode::ReadHoldingRegisters;
    uint16_t memoryAddress;
    uint16_t memoryLength;

    const std::array<uint8_t, 12>& toBuffer();

private:
    std::array<uint8_t, 12> _buffer;
};

} // namespace modbus
} // namespace uvw_net
