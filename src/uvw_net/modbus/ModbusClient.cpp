#include "ModbusClient.h"

#include <cstring>
#include <iostream>
#include <memory>

#include <uvw/loop.h>
#include <uvw/tcp.h>

#include "ModbusException.h"

namespace uvw_net {
namespace modbus {

std::shared_ptr<ModbusClient> ModbusClient::create() {
    // TODO: make_shared won't work here. New does. Is this ok?
    return std::shared_ptr<ModbusClient>(new ModbusClient());
    //return std::make_shared<ModbusThing>(host);
}

ModbusClient::ModbusClient() {
    //_tcpClient->keep_alive(true, uvw::tcp_handle::time{10});
    //close_event, connect_event, data_event, end_event, error_event, listen_event, shutdown_event, write_event
    _tcpClient->on<uvw::close_event>([this](const uvw::close_event& ev, uvw::tcp_handle& tcpClient) {
        publish(ev);
    });
    _tcpClient->on<uvw::connect_event>([this](const uvw::connect_event& ev, uvw::tcp_handle& tcpClient) {
        publish(ev);
    });
    _tcpClient->on<uvw::error_event>([this](const uvw::error_event& ev, uvw::tcp_handle& tcpClient) {
        publish(ev);
    });
    _tcpClient->on<uvw::shutdown_event>([this](const uvw::shutdown_event&, uvw::tcp_handle& tcpClient) {
        // TODO: why do we actually emit timeout event here?
        publish(uvw::error_event{(int)UV_ETIMEDOUT});
    });

    _tcpClient->on<uvw::data_event>([this](const uvw::data_event& event, uvw::tcp_handle& tcpClient) {
        // data_events can be emitted several times without carrying the whole
        // modbus message. So, we collect everything in a readBuffer.
        const auto size = _readBuffer.size();
        _readBuffer.resize(_readBuffer.size() + event.length);
        std::memcpy(_readBuffer.data()+size, event.data.get(), event.length);
        // Check if we can create a valid ModbusResponse out of our readBuffer.
        auto response = ModbusResponse::fromBuffer(_readBuffer.data(), _readBuffer.size());
        if (!response) {
            // No valid readBuffer, yet. Continue reading.
            return;
        }
        if (response->exceptionCode != ModbusExceptionCode::Illegal) {
            publish(ModbusException{response->unitId, response->exceptionCode});
        } else if (_transactionsUserData.contains(response->transactionId)) {
            response->userData = _transactionsUserData[response->transactionId];
            publish(*response);
        } else {
            //publish(WarningEvent::UnknownResponse);
            publish(*response);
        }

        _transactionsUserData.erase(response->transactionId);
        _readBuffer.clear();
    });
    // EOS handler
    _tcpClient->on<uvw::end_event>([this](const uvw::end_event&, uvw::tcp_handle&) {
        // Note: occassionally, we receive an EOF. Then we want to reconnect.
        _tcpClient->init();
        _tcpClient->connect(_ip, _port);
    });
    // Start reading after writing
    _tcpClient->on<uvw::write_event>([this](const uvw::write_event&, uvw::tcp_handle& tcpClient) {
        _readBuffer.clear();
        tcpClient.read();
    });
}

ModbusClient::~ModbusClient() {
    // Disconnects all listeners
    _tcpClient->reset();
    // Close the handle and release resources
    _tcpClient->close();
}

const std::string& ModbusClient::ip() const {
    return _ip;
}

uint16_t ModbusClient::port() const {
    return _port;
}

void ModbusClient::connect(const std::string& ip, uint16_t port) {
    _ip = ip;
    _port = port;
    _tcpClient->connect(_ip, _port);
}

void ModbusClient::readHoldingRegisters(uint8_t unitId, uint16_t address, uint16_t length, uint16_t userData) {
    _request.unitId = unitId;
    _request.memoryAddress = address;
    _request.memoryLength = length;
    const auto& buffer = _request.toBuffer();
    _transactionsUserData[_request.transactionId] = userData;
    _tcpClient->write((char*)buffer.data(), buffer.size());
    _tcpClient->read();
}

} // namespace modbus
} // namespace uvw_net
