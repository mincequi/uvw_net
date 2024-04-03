#pragma once

#include <uvw_net/sunspec/SunSpecClient.h>

namespace uvw_net {
namespace sunspec {

class SunSpecDiscovery final: public uvw::emitter<SunSpecDiscovery, /*SunSpecUnitId,*/ SunSpecClientPtr> {
public:
    SunSpecDiscovery();
    ~SunSpecDiscovery();

    void discover(modbus::ModbusClientPtr thing);

private:
    SunSpecClientPtr takeCandidate(const SunSpecClient& thing);

    std::list<SunSpecClientPtr> _candidates;
};

using SunSpecDiscoveryPtr = std::shared_ptr<SunSpecDiscovery>;

} // namespace sunspec
} // namespace uvw_net
