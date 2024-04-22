#pragma once

#include <map>
#include <set>

#include <uvw_net/sunspec/SunSpecBlock.h>
#include <uvw_net/sunspec/SunSpecDataPoint.h>
#include <uvw_net/sunspec/SunSpecMeasuredValue.h>
#include <uvw_net/sunspec/SunSpecModel.h>

namespace uvw_net {
namespace sunspec {

template <typename T0, typename ... Ts>
std::ostream& operator<< (std::ostream& os, const std::variant<T0, Ts...>& v) {
    std::visit([&](auto&& arg) { os << arg; }, v);
    return os;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec);
std::ostream& operator<<(std::ostream& os, const sunspec::SunSpecModel& model);
template <class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& set);
template <class T>
std::ostream& operator<<(std::ostream& os, const sunspec::SunSpecBlock<T>& value);

} // namespace sunspec
} // namespace uvw_net
