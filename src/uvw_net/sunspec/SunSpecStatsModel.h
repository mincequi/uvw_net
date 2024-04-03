#pragma once

#include <map>

#include "SunSpecDataPoint.h"
#include "SunSpecStatsValue.h"
#include "SunSpecTypes.h"

namespace uvw_net {
namespace sunspec {

class SunSpecModel;

class StatsModel {
public:
    StatsModel();

    StatsModel& operator=(const SunSpecModel& model);

    uint16_t modelId() const;

    bool isDirty() const;

    const std::map<DataPoint, StatsValue>& values() const;

private:
    uint16_t m_modelId = 0;
    bool m_isDirty = false;
    std::map<DataPoint, StatsValue> m_values;
};

} // namespace sunspec
} // namespace uvw_net
