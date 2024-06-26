#include "SunSpecElgrisSmartMeterModelFactory.h"

#include <cmath>

#include <uvw_net/sunspec/SunSpecLogger.h>
#include <uvw_net/sunspec/SunSpecModel.h>

namespace uvw_net {
namespace sunspec {

bool ElgrisSmartMeterModelFactory::updateFromBuffer(SunSpecModel& model, const std::vector<uint16_t>& buffer) {
    if (buffer.size() != 105) {
        return false;
    }

    const int16_t sunssf = *(int16_t*)&buffer.at(20) * -1;
    if (sunssf != 1) {
        //LOG_S(INFO) << "ElgrisSmartMeter> scale factor: " << sunssf;
    }
    const int32_t totalActivePower = *(int16_t*)&buffer.at(16) * pow(10.0, sunssf);

    model._values[sunspec::totalActiveAcPower] = totalActivePower;
    model._values[sunspec::totalExportedActiveEnergy] = (int32_t)round((buffer.at(36) + (buffer.at(37) << 16)) / 100.0) * 100.0;
    model._values[sunspec::totalImportedActiveEnergy] = (int32_t)round((buffer.at(44) + (buffer.at(45) << 16)) / 100.0) * 100.0;

    return true;
}

} // namespace sunspec
} // namespace uvw_net
