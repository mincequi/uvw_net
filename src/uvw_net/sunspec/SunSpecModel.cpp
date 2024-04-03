#include "SunSpecModel.h"

namespace uvw_net {
namespace sunspec {

uint16_t SunSpecModel::modelId() const {
    return _modelId;
}

const std::map<DataPoint, LiveValue>& SunSpecModel::values() const {
    return _values;
}

bool SunSpecModel::operator==(const SunSpecModel& other) const {
    return values() == other.values();
}

bool SunSpecModel::operator!=(const SunSpecModel& other) const {
    return values() != other.values();
}

LiveValue& SunSpecModel::operator[](DataPoint i) {
    return _values[i];
}

} // namespace sunspec
} // namespace uvw_net
