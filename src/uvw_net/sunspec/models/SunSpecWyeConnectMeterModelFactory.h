#pragma once

#include <cstdint>
#include <vector>

namespace uvw_net {
namespace sunspec {

class SunSpecModel;

class WyeConnectMeterModelFactory {
public:
    static bool updateFromBuffer(SunSpecModel& model,
                                 const std::vector<uint16_t>& buffer);
};

} // namespace sunspec
} // namespace uvw_net
