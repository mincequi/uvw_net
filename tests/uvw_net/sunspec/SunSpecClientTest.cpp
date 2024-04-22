#include <catch2/catch_test_macros.hpp>

#define protected public
#define private public
#include <uvw_net/modbus/ModbusClient.h>
#include <uvw_net/sunspec/SunSpecClient.h>

using namespace uvw_net::modbus;
using namespace uvw_net::sunspec;

TEST_CASE("SunSpecClient publishes events", "[sunspec][SunSpecClient]") {
    auto modbusClient = ModbusClient::create();
    SunSpecClient cut(modbusClient);

    bool close_event = false;
    bool connect_event = false;
    bool error_event = false;
    bool modbus_exception = false;
    uint16_t sunspecModel = 0;

    cut.on<uvw::close_event>([&](const auto&, const auto&) {
        close_event = true;
    });
    cut.on<uvw::connect_event>([&](const auto&, const auto&) {
        connect_event = true;
    });
    cut.on<uvw::error_event>([&](const auto&, const auto&) {
        error_event = true;
    });
    cut.on<ModbusException>([&](const auto&, const auto&) {
        modbus_exception = true;
    });
    cut.on<SunSpecModel>([&](const SunSpecModel& model, const auto&) {
        sunspecModel = model.modelId();
    });

    SECTION("close_event") {
        modbusClient->publish(uvw::close_event());
        REQUIRE(close_event);
        REQUIRE(!connect_event);
        REQUIRE(!error_event);
        REQUIRE(!modbus_exception);
        REQUIRE(sunspecModel == 0);
    }

    SECTION("connect_event") {
        modbusClient->publish(uvw::connect_event());
        REQUIRE(!close_event);
        REQUIRE(connect_event);
        REQUIRE(!error_event);
        REQUIRE(!modbus_exception);
        REQUIRE(sunspecModel == 0);
    }

    SECTION("error_event") {
        modbusClient->publish(uvw::error_event(0));
        REQUIRE(!close_event);
        REQUIRE(!connect_event);
        REQUIRE(error_event);
        REQUIRE(!modbus_exception);
        REQUIRE(sunspecModel == 0);
    }

    SECTION("ModbusException") {
        modbusClient->publish(ModbusException());
        REQUIRE(!close_event);
        REQUIRE(!connect_event);
        REQUIRE(!error_event);
        REQUIRE(modbus_exception);
        REQUIRE(sunspecModel == 0);
    }

    SECTION("ModbusResponse from ReadHeader with invalid header") {
        ModbusResponse response;
        response.userData = SunSpecClient::OperationId::ReadHeader;
        response.values.resize(4);
        response.values[3] = 65;
        modbusClient->publish(response);
        REQUIRE(cut._headerLength == 65);
        REQUIRE(close_event);
        REQUIRE(!connect_event);
        REQUIRE(!error_event);
        REQUIRE(!modbus_exception);
        REQUIRE(sunspecModel == 0);
    }

    SECTION("ModbusResponse from ReadHeader with valid header") {
        ModbusResponse response;
        response.userData = SunSpecClient::OperationId::ReadHeader;
        response.values.resize(4);
        response.values[0] = 0x5375;
        response.values[1] = 0x6e53;
        response.values[2] = 0x0001;
        response.values[3] = 65;
        modbusClient->publish(response);
        REQUIRE(cut._headerLength == 65);
        REQUIRE(!close_event);
        REQUIRE(!connect_event);
        REQUIRE(!error_event);
        REQUIRE(!modbus_exception);
        REQUIRE(sunspecModel == 0);
        REQUIRE(modbusClient->_request.memoryAddress == 40004);
    }

    SECTION("ModbusResponse from ReadModelEntry with a model entry") {
        ModbusResponse response;
        response.values.resize(2);
        modbusClient->publish(response);
        REQUIRE(!close_event);
        REQUIRE(!connect_event);
        REQUIRE(!error_event);
        REQUIRE(!modbus_exception);
        REQUIRE(sunspecModel == 0);
    }

    SECTION("ModbusResponse from ReadModelEntry with final model entry") {
        ModbusResponse response;
        response.values.resize(2);
        response.values[0] = 0xFFFF;
        modbusClient->publish(response);
        REQUIRE(!close_event);
        REQUIRE(connect_event);
        REQUIRE(!error_event);
        REQUIRE(!modbus_exception);
        REQUIRE(sunspecModel == 0);
    }
}
