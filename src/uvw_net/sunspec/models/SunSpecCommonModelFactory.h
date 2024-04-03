#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace uvw_net {
namespace sunspec {

class SunSpecModel;

class SunSpecCommonModelFactory {
public:
    static bool updateFromBuffer(SunSpecModel& model, const std::vector<uint16_t>& buffer);

private:
    static std::string readString(const uint16_t* begin, uint8_t length);
};

} // namespace sunspec
} // namespace uvw_net
