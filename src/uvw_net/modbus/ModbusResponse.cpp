#include "ModbusResponse.h"

#include <cstring>

#include <magic_enum.hpp>

namespace uvw_net {
namespace modbus {

ModbusResponse::ModbusResponse() {
}

std::optional<ModbusResponse> ModbusResponse::fromBuffer(const uint8_t buffer[], int length) {
    if (length < 8) {
        return {};
    }

    auto p = buffer;
    ModbusResponse response;
    uint16_t transactionId_; std::memcpy(&transactionId_, p, 2); p += 2;
    response.transactionId = ((transactionId_ & 0xff) << 8) | ((transactionId_ & 0xff00) >> 8);
    uint16_t protocolId_; std::memcpy(&protocolId_, p, 2); p += 2;
    response.protocolId = ((protocolId_ & 0xff) << 8) | ((protocolId_ & 0xff00) >> 8);
    uint16_t length_;
    std::memcpy(&length_, p, 2); p += 2;
    length_ = ((length_ & 0xff) << 8) | ((length_ & 0xff00) >> 8);
    if (length < length_ + 6) {
        return {};
    }

    response.unitId = *p; p += 1;
    auto functionCode = magic_enum::enum_cast<ModbusFunctionCode>(*p); p += 1;
    if (functionCode.has_value()) {
        response.functionCode = functionCode.value();
    } else {
        response.exceptionCode = magic_enum::enum_cast<ModbusExceptionCode>(*p).value_or(ModbusExceptionCode::Unknown);
        return response;
    }

    uint8_t size = *p/2; p += 1;
    response.values.resize(size);
    for (uint8_t i = 0; i < size; ++i) {
        uint16_t v;
        std::memcpy(&v, p, 2);
        response.values[i] = ((v & 0xff) << 8) | ((v & 0xff00) >> 8);
        p += 2;
    }

    return response;
}

std::vector<uint8_t> ModbusResponse::toBuffer() const {
    // Determine the length of the response (2 bytes for byte count + 2 bytes per register value)
    uint16_t length = 3 + values.size() * 2;

    // Create the buffer for the response with the MBAP header and the data
    std::vector<uint8_t> buffer(6 + length);

    // MBAP header
    buffer[0] = (transactionId >> 8) & 0xFF; // Transaction ID (MSB)
    buffer[1] = transactionId & 0xFF;       // Transaction ID (LSB)
    buffer[2] = (protocolId >> 8) & 0xFF;   // Protocol ID (MSB)
    buffer[3] = protocolId & 0xFF;          // Protocol ID (LSB)
    buffer[4] = (length >> 8) & 0xFF;       // Length (MSB)
    buffer[5] = length & 0xFF;              // Length (LSB)

    // Unit ID and function code
    buffer[6] = unitId;                     // Unit ID
    buffer[7] = (uint8_t)functionCode;      // Function Code

    // Byte count (number of bytes following the function code, 2 bytes per register)
    buffer[8] = values.size() * 2;

    // Insert register values into the buffer
    for (size_t i = 0; i < values.size(); i++) {
        uint16_t value = values[i];
        buffer[9 + (i * 2)] = (value >> 8) & 0xFF; // High byte of the register value
        buffer[10 + (i * 2)] = value & 0xFF;       // Low byte of the register value
    }

    return buffer;
}

} // namespace modbus
} // namespace uvw_net
