#include "ModbusRequest.h"

#include <cstring>

namespace uvw_net {
namespace modbus {

ModbusRequest::ModbusRequest() {
}

const std::array<uint8_t, 12>& ModbusRequest::toBuffer() {
    static const uint16_t protocolId = 0x00;
    static const uint8_t length = 0x06;

    ++transactionId;

    auto p = _buffer.data();
    _buffer[0] = (uint8_t)((transactionId >> 8) & 0xFF);
    _buffer[1] = (uint8_t)(transactionId & 0xFF);
    p += 2;
    std::memcpy(p, &protocolId, 2); p += 2;
    *p = 0; p += 1;
    *p = length; p += 1;
    *p = unitId; p += 1;
    *p = (uint8_t)functionCode; p += 1;
    *p = (memoryAddress & 0xff00) >> 8; p += 1;
    *p = (memoryAddress & 0xff); p += 1;
    *p = (memoryLength & 0xff00) >> 8; p += 1;
    *p = (memoryLength & 0xff);

    return _buffer;
}

} // namespace modbus
} // namespace uvw_net
