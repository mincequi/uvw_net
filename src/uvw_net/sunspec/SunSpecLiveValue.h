#pragma once

#include <variant>

#include <uvw_net/sunspec/SunSpecBlock.h>
#include <uvw_net/sunspec/SunSpecTypes.h>

namespace uvw_net {
namespace sunspec {

using LiveValue = std::variant<
    uint32_t,   // timestamp
    InverterOperatingStatus,
    InverterEvents,
    int32_t,    //
    double,     // regular value
    std::vector<SunSpecBlock<double>>,    // block arrays
    std::string>;

} // namespace sunspec
} // namespace uvw_net
