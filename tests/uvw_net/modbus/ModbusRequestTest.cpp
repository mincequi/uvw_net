#include <catch2/catch_test_macros.hpp>

#include <uvw_net/modbus/ModbusRequest.h>

using namespace uvw_net::modbus;

TEST_CASE("ModbusRequest", "[modbus][ModbusRequestTest]") {
    ModbusRequest request;
    request.transactionId = 0x0102;
    request.unitId = 0x9A;
    request.functionCode = ModbusFunctionCode::ReportSlaveID;
    request.memoryAddress = 0xDE;
    request.memoryLength = 0xAD;

    const auto buffer = request.toBuffer();
    REQUIRE(buffer.size() == 12);
    REQUIRE(buffer[0] == 0x01); // transactionId is incremented
    REQUIRE(buffer[1] == 0x03);
    REQUIRE(buffer[2] == 0x00); // protocolId
    REQUIRE(buffer[3] == 0x00);
    REQUIRE(buffer[4] == 0x00); // length
    REQUIRE(buffer[5] == 0x06);
    REQUIRE(buffer[6] == 0x9A); // unitId
    REQUIRE(buffer[7] == 0x11); // functionCode
    REQUIRE(buffer[8] == 0x00);
    REQUIRE(buffer[9] == 0xDE);
    REQUIRE(buffer[10] == 0x00);
    REQUIRE(buffer[11] == 0xAD);
}
