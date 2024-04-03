#pragma once

#include <set>
#include <string>
#include <variant>
#include <vector>

#include <uvw_net/sunspec/SunSpecLiveValue.h>
#include <uvw_net/sunspec/SunSpecMeasuredValue.h>
#include <uvw_net/sunspec/SunSpecStatsValue.h>
#include <uvw_net/sunspec/SunSpecTypes.h>

namespace uvw_net {
namespace sunspec {

class StatsValue {
public:
    StatsValue& operator=(const LiveValue& v);
    StatsValue& operator|=(const InverterOperatingStatus& v);
    StatsValue& operator|=(const InverterEvents& v);

    bool isDirty() const;

    double minValue() const;
    double maxValue() const;

private:
    std::variant<
            uint32_t,
            std::set<InverterOperatingStatus>,
            InverterEvents,
            //MeasuredValue<int32_t>,
            MeasuredValue<double>,
            std::vector<SunSpecBlock<MeasuredValue<double>>>> m_variant;

    bool m_isDirty = false;

    friend std::ostream& operator<<(std::ostream& os, const sunspec::StatsValue& value);
    friend class StatsModel;
};

} // namespace sunspec
} // namespace uvw_net
