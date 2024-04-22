#include <catch2/catch_test_macros.hpp>

#define private public
#include <uvw_net/modbus/ModbusDiscovery.h>

using namespace uvw_net::modbus;

// It is impossible to reliable test the ModbusDiscovery class, because it is
// highly dependent on the actual Hardware.
TEST_CASE("ModbusDiscovery", "[modbus][ModbusDiscoveryTest]") {
}
