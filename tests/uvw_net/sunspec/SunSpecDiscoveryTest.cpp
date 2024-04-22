#include <catch2/catch_test_macros.hpp>

#define protected public
#define private public
#include <uvw_net/modbus/ModbusClient.h>
#include <uvw_net/sunspec/SunSpecDiscovery.h>

using namespace uvw_net::modbus;
using namespace uvw_net::sunspec;

TEST_CASE("SunSpecDiscovery::discover", "[sunspec][SunSpecDiscovery]") {
    SunSpecClientPtr _candidate;

    SunSpecDiscovery cut;
    cut.on<SunSpecClientPtr>([&](const auto& candidate_, const auto&) {
        _candidate = candidate_;
    });

    auto modbusClient = ModbusClient::create();

    cut.discover(modbusClient);

    auto candidate = cut._candidates.front();
    REQUIRE(candidate->_modbusClient == modbusClient);
    REQUIRE(candidate->_unitId == 1);
    REQUIRE(candidate->_unitIdx == 1);

    SECTION("candidate closes") {
        candidate->publish(uvw::close_event());
        REQUIRE(cut._candidates.empty());
        REQUIRE(_candidate == nullptr);
    }
    SECTION("candidate errors") {
        candidate->publish(uvw::error_event(0));
        REQUIRE(cut._candidates.empty());
        REQUIRE(_candidate == nullptr);
    }
    SECTION("candidate connects") {
        candidate->publish(uvw::connect_event());
        REQUIRE(cut._candidates.empty());
        REQUIRE(_candidate == candidate);
    }
    SECTION("candidate excepts") {
        candidate->publish(ModbusException());
        REQUIRE(candidate->_unitId == 240);
        REQUIRE(candidate->_unitIdx == 2);
        REQUIRE(_candidate == nullptr);
    }
}
