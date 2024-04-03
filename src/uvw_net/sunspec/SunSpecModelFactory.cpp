#include "SunSpecModelFactory.h"

#include <functional>
#include <map>

//#include <common/Logger.h>

#include <uvw_net/sunspec/SunSpecModel.h>
#include <uvw_net/sunspec/models/SunSpecCommonModelFactory.h>
#include <uvw_net/sunspec/models/SunSpecElgrisSmartMeterModelFactory.h>
#include <uvw_net/sunspec/models/SunSpecInverterModelFactory.h>
#include <uvw_net/sunspec/models/SunSpecMpptInverterExtensionModelFactory.h>
#include <uvw_net/sunspec/models/SunSpecWyeConnectMeterModelFactory.h>

namespace uvw_net {
namespace sunspec {

static std::map<uint16_t, std::function<void(SunSpecModel& model,
                                             const std::vector<uint16_t>& buffer)>> s_factories = {
    //{ 1,   &SunSpecCommonModelFactory::updateFromBuffer },
    { SunSpecModel::Id::InverterSinglePhase, &InverterModelFactory::updateFromBuffer },
    { SunSpecModel::Id::InverterThreePhase, &InverterModelFactory::updateFromBuffer },
    { SunSpecModel::Id::InverterMpptExtension, &MpptInverterExtensionModelFactory::updateFromBuffer },
    { SunSpecModel::Id::MeterWyeConnectThreePhase, &WyeConnectMeterModelFactory::updateFromBuffer },
};

bool ModelFactory::updateFromBuffer(std::map<uint16_t, SunSpecModel>& models,
                                    uint16_t modelId,
                                    const std::vector<uint16_t>& buffer) {
    if (buffer.empty() || s_factories.count(modelId) == 0) {
        return false;
    }

    if (models.count(modelId) == 0) {
        models[modelId]._modelId = modelId;
    }

    // Special handling for elgris smart meter
    if (models[1].values().count(sunspec::Manufacturer) &&
            std::get<std::string>(models[1].values().at(sunspec::Manufacturer)) == "elgris") {
        ElgrisSmartMeterModelFactory::updateFromBuffer(models[modelId], buffer);
    } else {
        s_factories.at(modelId)(models[modelId], buffer);
    }

    return true;
}

} // namespace sunspec
} // namespace uvw_net
