#include "SunSpecDiscovery.h"

#include <cassert>
#include <iostream>

#include <magic_enum_iostream.hpp>

#include <uvw_net/modbus/ModbusClient.h>

namespace uvw_net {
namespace sunspec {

using namespace modbus;
using magic_enum::iostream_operators::operator<<;

SunSpecDiscovery::SunSpecDiscovery() {
}

SunSpecDiscovery::~SunSpecDiscovery() {
}

void SunSpecDiscovery::discover(modbus::ModbusClientPtr thing) {
    // SunSpecThing will automatically scan unitIds when constructed
    auto candidate = std::make_shared<SunSpecClient>(thing);
    candidate->on<uvw::close_event>([this](const auto&, const SunSpecClient& candidate){
        takeCandidate(candidate);
    });
    candidate->on<uvw::connect_event>([this](const auto&, const SunSpecClient& candidate) {
        publish(takeCandidate(candidate));
    });
    candidate->on<uvw::error_event>([this](const auto&, const SunSpecClient& candidate) {
        takeCandidate(candidate);
    });
    candidate->on<ModbusException>([](const ModbusException& exception, SunSpecClient& thing) {
        //std::cerr << thing.host() << "> unit: " << (int)exception.uintId << ", modbusException: " << exception.functionCode << std::endl;
        if (thing.sunSpecId().empty()) {
            thing.probeNextUnitId();
        }
    });
    //candidate->on<SunSpecUnitId>([this](const auto& id, const SunSpecThing&) {
    //    publish(id);
    //});
    candidate->probeNextUnitId();
    _candidates.push_back(std::move(candidate));
}

SunSpecClientPtr SunSpecDiscovery::takeCandidate(const SunSpecClient& candidate_) {
    assert(!_candidates.empty());
    auto it = std::find_if(_candidates.begin(), _candidates.end(), [&](const auto& c) {
        return c.get() == &candidate_;
    });
    assert(it != _candidates.end());

    // Steal candidate from container
    auto candidate = *it;
    _candidates.erase(it);

    // Disconnect signals, since we are handing-off this object
    candidate->reset();

    return candidate;
}

} // namespace sunspec
} // namespace uvw_net

