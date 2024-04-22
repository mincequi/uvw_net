#include <catch2/catch_test_macros.hpp>

#include <uvw_net/modbus/ModbusResponse.h>

using namespace uvw_net::modbus;

TEST_CASE("ModbusResponse", "[modbus][ModbusResponseTest]") {
    ModbusResponse response;
    response.transactionId = 0x0102;
    response.protocolId = 0x0304;
    response.unitId = 0x9A;
    response.functionCode = ModbusFunctionCode::WriteSingleRegister;
    response.values = {0x0102, 0x0304, 0x0506};

    const auto buffer = response.toBuffer();
    REQUIRE(buffer.size() == 15);
    REQUIRE(buffer[0] == 0x01); // transactionId
    REQUIRE(buffer[1] == 0x02);
    REQUIRE(buffer[2] == 0x03); // protocolId
    REQUIRE(buffer[3] == 0x04);
    REQUIRE(buffer[4] == 0x00); // length
    REQUIRE(buffer[5] == 0x09);
    REQUIRE(buffer[6] == 0x9A); // unitId
    REQUIRE(buffer[7] == 0x06); // functionCode
    REQUIRE(buffer[8] == 0x06); // values size
    REQUIRE(buffer[9] == 0x01);
    REQUIRE(buffer[10] == 0x02);
    REQUIRE(buffer[11] == 0x03);
    REQUIRE(buffer[12] == 0x04);
    REQUIRE(buffer[13] == 0x05);
    REQUIRE(buffer[14] == 0x06);

    const auto response2 = ModbusResponse::fromBuffer(buffer.data(), buffer.size());
    REQUIRE(response2.has_value());
    REQUIRE(response2->transactionId == response.transactionId);
    REQUIRE(response2->protocolId == response.protocolId);
    REQUIRE(response2->unitId == response.unitId);
    REQUIRE(response2->functionCode == response.functionCode);
    REQUIRE(response2->values == response.values);
}
