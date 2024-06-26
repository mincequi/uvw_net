#pragma once

#include <bitset>
#include <cstdint>
#include <sstream>

namespace uvw_net {
namespace sunspec {

enum InverterOperatingStatus : uint16_t {
    invalid         = 0,
    off             = 1,
    sleeping        = 2,
    starting        = 3,
    mpp             = 4,
    throttled       = 5,
    shuttingDown    = 6,
    error           = 7,
    service         = 8
};

enum InverterEvent : uint16_t {
    groundingError      = 0,
    dcOvervoltage       = 1,
    acDisconnected      = 2,
    dcDisconnected      = 3,
    gridDisconnected    = 4,
    enclosureOpen       = 5,
    shutdownManually    = 6,
    overTemperature     = 7,
    overFrequency       = 8,
    underFrequency      = 9,
    acOverVoltage       = 10,
    acUnderVoltage      = 11,
    stringFuseDefective = 12,
    underTemperature    = 13,
    storageOrCommunicationError = 14,
    hardwareTestError   = 15
};
class InverterEvents : public std::bitset<16> {
    using bitset::bitset;
};
std::ostream& operator<<(std::ostream& os, const InverterEvents& value);

} // namespace sunspec
} // namespace uvw_net
