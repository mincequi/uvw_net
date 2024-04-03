#include "ModbusResponse.h"

#include <cstring>

#include <magic_enum.hpp>

namespace uvw_net {
namespace modbus {

std::optional<ModbusResponse> ModbusResponse::fromBuffer(const uint8_t buffer[], int length) {
    if (length < 8) {
        return {};
    }

    auto p = buffer;
    ModbusResponse response;

    std::memcpy(&response.transactionId, p, 2); p += 2;
    std::memcpy(&response.protocolId, p, 2); p += 2;
    uint16_t length_;
    std::memcpy(&length_, p, 2); p += 2;
    length_ = ((length_ & 0xff) << 8) | ((length_ & 0xff00) >> 8);
    if (length < length_ + 6) {
        return {};
    }

    response.unitId = *p; p += 1;
    response.functionCode = *p; p += 1;
    if (response.functionCode & 0x80) {
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

ModbusResponse::ModbusResponse() {
}

} // namespace modbus
} // namespace uvw_net
