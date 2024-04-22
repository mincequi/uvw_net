#pragma once

#include <cstdint>

namespace uvw_net {
namespace modbus {

enum class ModbusExceptionCode : uint8_t {
    Illegal = 0x00,
    IllegalFunction = 0x01,
    IllegalDataAddress = 0x02,
    IllegalDataValue = 0x03,
    ServerDeviceFailure = 0x04,
    Acknowledge = 0x05,
    ServerDeviceBusy = 0x06,
    MemoryParityError = 0x08,
    GatewayPathUnavailable = 0x0A,
    GatewayTargetDeviceFailedToRespond = 0x0B,

    Unknown = 0xFF
};

struct ModbusException {
    uint8_t unitId;
    ModbusExceptionCode exceptionCode;
};

} // namespace modbus
} // namespace uvw_net

