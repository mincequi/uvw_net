#pragma once

#include <map>

#include <uvw/emitter.h>
#include <uvw/tcp.h>

#include <uvw_net/common/WarningEvent.h>
#include <uvw_net/modbus/ModbusException.h>
#include <uvw_net/modbus/ModbusRequest.h>
#include <uvw_net/modbus/ModbusResponse.h>

namespace uvw_net {
namespace modbus {

class ModbusClient final: public std::enable_shared_from_this<ModbusClient>,
        public uvw::emitter<ModbusClient,
        uvw::close_event,
        uvw::connect_event,
        ModbusException,
        ModbusResponse> {
public:
    static std::shared_ptr<ModbusClient> create();
    virtual ~ModbusClient();

    const std::string& ip() const;
    uint16_t port() const;

    void connect(const std::string& ip, uint16_t port = 502);
    void disconnect();
    bool isConnected() const;

    void readHoldingRegisters(uint8_t uintId, uint16_t address, uint16_t length, uint16_t userData = 0);

private:
    ModbusClient();

    std::string _ip;
    uint16_t _port = 502;
    std::shared_ptr<uvw::tcp_handle> _tcpClient = uvw::loop::get_default()->resource<uvw::tcp_handle>();
    ModbusRequest _request;
    std::map<uint16_t, uint16_t> _transactionsUserData;
    std::vector<uint8_t> _readBuffer;

    //std::shared_ptr<uvw::timer_handle> _timer;
};

using ModbusClientPtr = std::shared_ptr<ModbusClient>;

} // namespace modbus
} // namespace uvw_net
