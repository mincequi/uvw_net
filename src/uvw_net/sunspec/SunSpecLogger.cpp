#include "SunSpecLogger.h"

#include <vector>

#include <magic_enum_iostream.hpp>

#include <uvw_net/common/Util.h>
#include <uvw_net/sunspec/SunSpecMeasuredValue.h>

using namespace magic_enum::ostream_operators;

namespace uvw_net {
namespace sunspec {

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        os << *it;
        if (it != vec.end()-1) {
            os << ",";
        }
    }
    os << "]";

    return os;
}

std::ostream& operator<<(std::ostream& os, const sunspec::SunSpecModel& model) {
    os << "{";
    for (auto it = model.values().cbegin(); it != model.values().cend(); ++it) {
        os << "\"" << util::toString(it->first) << "\":" << it->second;
        if (std::distance(it, model.values().cend()) > 1) {
            os << ",";
        }
    }
    os << "}";
    return os;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& set) {
    os << "[";
    for (auto it = set.begin(); it != set.end(); ++it) {
        os << *it << ",";
    }

    if (!set.empty()) {
        os.seekp(-1, os.cur);
    }

    os << "]";

    return os;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const sunspec::SunSpecBlock<T>& value) {
    os << "{";
    for (auto it = value.data().cbegin(); it != value.data().cend(); ++it) {
        os << "\"" << util::toString(it->first) << "\":" << it->second;
        //os << "\"" << it->first << "\":" << it->second;
        if (std::distance(it, value.data().cend()) > 1) {
            os << ",";
        }
    }
    os << "}";
    return os;
}

template std::ostream& operator<<(std::ostream& os, const std::vector<sunspec::SunSpecBlock<double>>& value);

} // namespace sunspec
} // namespace uvw_net
