#include "SunSpecInverterModelFactory.h"

#include <cmath>
#include <vector>

#include <uvw_net/sunspec/SunSpecModel.h>

namespace uvw_net {
namespace sunspec {

bool InverterModelFactory::updateFromBuffer(SunSpecModel& model,
                                            const std::vector<uint16_t>& buffer) {
    if (buffer.size() != 50) {
        return false;
    }

    const auto sfPower = *(int16_t*)&buffer.at(13);
    int32_t totalActiveAcPower = *(int16_t*)&buffer.at(12);
    totalActiveAcPower = totalActiveAcPower < 0 ? 0 : totalActiveAcPower;
    totalActiveAcPower *= pow(10.0, sfPower);

    const auto sfYield = *(int16_t*)&buffer.at(24);
    int32_t totalYield = (buffer.at(22) << 16) + buffer.at(23);
    totalYield *= pow(10.0, sfYield);

    //model._values[sunspec::timestamp] = timestamp;
    model._values[sunspec::totalActiveAcPower] = totalActiveAcPower;
    model._values[sunspec::totalExportedActiveEnergy] = (int32_t)(round(totalYield/100.0)*100.0);
    model._values[sunspec::operatingStatus] = static_cast<InverterOperatingStatus>(buffer.at(36));
    model._values[sunspec::events] = InverterEvents(buffer.at(38));

    return true;
}

} // namespace sunspec
} // namespace uvw_net
