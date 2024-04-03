#pragma once

#include <cstdint>

namespace uvw_net {

enum class WarningEvent : uint8_t {
    IllegalSize,
    UnknownResponse
};

} // namespace uvw_net
