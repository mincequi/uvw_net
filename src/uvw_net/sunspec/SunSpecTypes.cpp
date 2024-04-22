#include "SunSpecTypes.h"

#include <magic_enum.hpp>

#include <uvw_net/sunspec/SunSpecMeasuredValue.h>

namespace uvw_net {
namespace sunspec {

std::ostream& operator<<(std::ostream& os, const InverterEvents& value) {
    os << "[";
    bool elementAdded = false;
    for (size_t i = 0; i < value.size(); ++i) {
        if (value.test(i)) {
            auto e = magic_enum::enum_cast<InverterEvent>(i);
            if (e.has_value()) {
                elementAdded = true;
                os << "\"" << magic_enum::enum_name(e.value()) << "\",";
            }
        }
    }
    if (elementAdded) {
        os.seekp(-1, os.cur);
    }
    os << "]";
    return os;
}

} // namespace sunspec
} // namespace uvw_net
