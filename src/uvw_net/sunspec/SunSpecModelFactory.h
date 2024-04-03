#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace uvw_net {
namespace sunspec {

class SunSpecModel;

class ModelFactory {
public:
    static bool updateFromBuffer(std::map<uint16_t, SunSpecModel>& models,
                                 uint16_t modelId,
                                 const std::vector<uint16_t>& buffer);
};

} // namespace sunspec
} // namespace uvw_net
