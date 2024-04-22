#pragma once

#include <magic_enum.hpp>

namespace uvw_net {
namespace util {

template <typename E>
std::string toString(E value) {
    std::string out { magic_enum::enum_name(value) };
    if (!out.empty()) out[0] = std::tolower(out[0]);
    return out;
}

void callLater(std::function<void()> f);

} // namespace util
} // namespace uvw_net
