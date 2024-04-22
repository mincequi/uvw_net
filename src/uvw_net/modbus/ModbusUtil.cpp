#include "ModbusUtil.h"

namespace uvw_net {
namespace modbus {

std::vector<char> ModbusUtil::createModbusExceptionBuffer(
    uint16_t transactionId,
    uint16_t protocolId,
    uint16_t length,
    uint8_t unitId,
    uint8_t functionCode,
    ModbusExceptionCode exceptionCode) {

    // Creating a vector with size 9 to accommodate the MBAP header and response
    std::vector<char> buffer(9);

    // Transaction ID (big-endian)
    buffer[0] = (transactionId >> 8) & 0xFF; // MSB
    buffer[1] = transactionId & 0xFF;       // LSB

    // Protocol ID (big-endian, usually 0)
    buffer[2] = (protocolId >> 8) & 0xFF;   // MSB
    buffer[3] = protocolId & 0xFF;          // LSB

    // Length (big-endian, length of the rest of the packet)
    buffer[4] = (length >> 8) & 0xFF;       // MSB
    buffer[5] = length & 0xFF;              // LSB

    // Unit ID
    buffer[6] = unitId;

    // Function code with exception flag (0x80)
    buffer[7] = functionCode | 0x80;

    // Exception code
    buffer[8] = (char)exceptionCode;

    return buffer;
}

} // namespace modbus
} // namespace uvw_net
