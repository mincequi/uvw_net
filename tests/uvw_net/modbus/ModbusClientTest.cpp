#include <catch2/catch_test_macros.hpp>

#define protected public
#define private public
#include <uvw_net/modbus/ModbusClient.h>
#include <uvw_net/modbus/ModbusUtil.h>

using namespace uvw;
using namespace uvw_net::modbus;

TEST_CASE("ModbusClient publishes uvw events", "[modbus][ModbusClient]") {
    bool closeEvent = false;
    bool connectEvent = false;
    int errorEvent = 0;

    ModbusClientPtr cut = ModbusClient::create();
    cut->on<close_event>([&](auto&, auto&) {
        closeEvent = true;
    });
    cut->on<connect_event>([&](auto&, auto&) {
        connectEvent = true;
    });
    cut->on<error_event>([&](auto& error, auto&) {
        errorEvent = error.code();
    });

    SECTION("close_event") {
        cut->_tcpClient->publish(close_event());
        REQUIRE(closeEvent);
    }
    SECTION("connect_event") {
        cut->_tcpClient->publish(connect_event());
        REQUIRE(connectEvent);
    }
    SECTION("error_event") {
        cut->_tcpClient->publish(error_event(1));
        REQUIRE(errorEvent == 1);
    }
    SECTION("shutdown_event") {
        cut->_tcpClient->publish(shutdown_event());
        REQUIRE(errorEvent == UV_ETIMEDOUT);
    }
}

TEST_CASE("ModbusClient publishes modbus events", "[modbus][ModbusClient]") {
    ModbusException _exception;
    ModbusResponse _response;

    ModbusClientPtr cut = ModbusClient::create();

    cut->on<ModbusException>([&](auto& ev, auto&) {
        _exception = ev;
    });
    cut->on<ModbusResponse>([&](auto& ev, auto&) {
        _response = ev;
    });

    SECTION("ModbusException") {
        std::vector<char> mockData = ModbusUtil::createModbusExceptionBuffer(1, 2, 3, 5, 6, ModbusExceptionCode::Acknowledge);
        auto buffer = std::make_unique<char[]>(mockData.size());
        std::copy(mockData.begin(), mockData.end(), buffer.get());
        data_event dataEvent(std::move(buffer), mockData.size());

        cut->_tcpClient->publish(std::move(dataEvent));
        REQUIRE(_exception.unitId == 5);
        REQUIRE(_exception.exceptionCode == ModbusExceptionCode::Acknowledge);
    }

    SECTION("ModbusResponse") {
        ModbusResponse response;
        response.transactionId = 1;
        response.protocolId = 2;
        response.unitId = 3;
        response.functionCode = ModbusFunctionCode::MaskWriteRegister;
        response.values = {7, 8, 9};

        auto mockData = response.toBuffer();
        auto buffer = std::make_unique<char[]>(mockData.size());
        std::copy(mockData.begin(), mockData.end(), buffer.get());
        data_event dataEvent(std::move(buffer), mockData.size());

        cut->_tcpClient->publish(std::move(dataEvent));
        REQUIRE(_response.transactionId == 1);
        REQUIRE(_response.protocolId == 2);
        REQUIRE(_response.unitId == 3);
        REQUIRE(_response.functionCode == ModbusFunctionCode::MaskWriteRegister);
        REQUIRE(_response.userData == 0);
        REQUIRE(_response.values == std::vector<uint16_t>({7, 8, 9}));
    }

    SECTION("ModbusResponse with userData") {
        ModbusResponse response;
        response.transactionId = 1;

        auto mockData = response.toBuffer();
        auto buffer = std::make_unique<char[]>(mockData.size());
        std::copy(mockData.begin(), mockData.end(), buffer.get());
        data_event dataEvent(std::move(buffer), mockData.size());

        cut->_transactionsUserData[1] = 4;
        cut->_tcpClient->publish(std::move(dataEvent));
        REQUIRE(_response.transactionId == 1);
        REQUIRE(_response.userData == 4);
    }
}

TEST_CASE("ModbusClient::connect", "[modbus][ModbusClient]") {
    ModbusClientPtr cut = ModbusClient::create();
    cut->connect("localhost", 1);
    REQUIRE(cut->ip() == "localhost");
    REQUIRE(cut->port() == 1);
}

TEST_CASE("ModbusClient::readHoldingRegisters", "[modbus][ModbusClient]") {
    ModbusClientPtr cut = ModbusClient::create();
    cut->readHoldingRegisters(1, 2, 3, 4);
    REQUIRE(cut->_request.unitId == 1);
    REQUIRE(cut->_request.memoryAddress == 2);
    REQUIRE(cut->_request.memoryLength == 3);
    REQUIRE(cut->_request.transactionId == 1);
    REQUIRE(cut->_transactionsUserData[1] == 4);
}
