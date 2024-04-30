#pragma once

#include <uvw_net/modbus/ModbusClient.h>
#include <uvw_net/sunspec/SunSpecLiveValue.h>
#include <uvw_net/sunspec/SunSpecModel.h>

namespace uvw_net {
namespace sunspec {

/*! @brief UnitId event. */
struct SunSpecUnitId {
    uint8_t id;
    std::string host;
};

/**
 * Implementation of a SunSpec thing via modbus
 *
 * At construction time this thing will poll any modbus unitId (in a strategic
 * order) for a valid SunSpec device. If it finds something it will check for
 * supported models.
 * A modbus host can actually house multiple modbus things (1 to 247). However,
 * we stop polling after finding the first valid unitId.
 * If we want to check for further unitIds, this has to be part of higher level
 * classes/implementations (e.g. FroniusDiscovery).
 */
class SunSpecClient final: public uvw::emitter<SunSpecClient,
        uvw::close_event,   // this device is closed (e.g. no sunspec header found).
        uvw::connect_event, // this device successfully initiated a connection
        modbus::ModbusException,    // for discovery (e.g. invalid id)
        //SunSpecUnitId,
        SunSpecModel> {
public:
    enum OperationId : uint16_t {
        ValidStart = 40000,
        ValidEnd = 49999,
        ReadHeader = 63000,
        ReadModelEntry = 63001
    };

    SunSpecClient(modbus::ModbusClientPtr modbusThing);
    ~SunSpecClient();

    void probeNextUnitId();

    const std::string& host() const;
    uint8_t unitId() const;

    std::string manufacturer() const;
    std::string product() const;
    std::string serial() const;
    const std::string& sunSpecId() const;

    const std::map<uint16_t, std::pair<uint16_t, uint16_t>>& models() const;

    bool readModel(uint16_t modelId);

private:
    void doRead();

    uint8_t nextUnitId() const;

    void readHeader(uint8_t id);
    void onReadHeader(const modbus::ModbusResponse& response);

    void readModelEntry(uint16_t address);
    void onReadModelEntry(const modbus::ModbusResponse& response);
    void addModelAddress(uint16_t modelId, uint16_t startAddress, uint16_t length);

    void readBlock(uint16_t modelId, uint16_t address, uint16_t size);
    void onReadBlock(const modbus::ModbusResponse& response);
    void parseBlock(uint16_t modelId, const std::vector<uint16_t>& buffer);

    static std::string toString(const sunspec::LiveValue& v);

    modbus::ModbusClientPtr _modbusClient;
    mutable uint8_t _unitId = 0;
    mutable uint8_t _unitIdx = 0;

    uint16_t _headerLength = 0;
    std::string _manufacturer;
    std::string _product;
    std::string _serial;
    std::string _sunSpecId;

    uint8_t _timeoutCount = 0;

    std::map<uint16_t, std::pair<uint16_t, uint16_t>> _blockAddresses;

    std::map<uint16_t, SunSpecModel> _models;

    friend class FroniusDiscovery;
};

using SunSpecClientPtr = std::shared_ptr<sunspec::SunSpecClient>;

} // namespace sunspec
} // namespace uvw_net
