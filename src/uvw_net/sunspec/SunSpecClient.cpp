#include "SunSpecClient.h"

#include <iostream>
#include <regex>

#include <magic_enum_iostream.hpp>

#include <uvw_net/sunspec/SunSpecLogger.h>
#include <uvw_net/sunspec/SunSpecModelFactory.h>
#include <uvw_net/sunspec/models/SunSpecCommonModelFactory.h>
#include <uvw_net/sunspec/models/SunSpecInverterModelFactory.h>
#include <uvw_net/sunspec/models/SunSpecMpptInverterExtensionModelFactory.h>
#include <uvw_net/sunspec/models/SunSpecWyeConnectMeterModelFactory.h>

namespace uvw_net {
namespace sunspec {

using magic_enum::iostream_operators::operator<<;
using namespace modbus;

SunSpecClient::SunSpecClient(modbus::ModbusClientPtr modbusClient)
    : _modbusClient(modbusClient->shared_from_this()) {
    _modbusClient->on<uvw::close_event>([this](const auto& ev, const auto&) {
        publish(ev);
    });
    _modbusClient->on<uvw::connect_event>([this](const auto& ev, const auto&) {
        // Note: connect_event shall only be published, when the header is read successfully.
        //publish(ev);
    });
    _modbusClient->on<uvw::error_event>([this](const auto& ev, const auto&) {
        publish(ev);
    });
    _modbusClient->on<ModbusException>([this](const ModbusException& exception, const auto&) {
        publish(exception);
    });
    _modbusClient->on<ModbusResponse>([this](const ModbusResponse& response, const auto&) {
        if (response.userData == ReadHeader) {
            //std::cout << host() << "> onReadHeader" << std::endl;
            onReadHeader(response);
        } else if (response.values.size() == 2) {
            //std::cout << host() << "> onReadModelEntry" << std::endl;
            onReadModelEntry(response);
        } else {
            //std::cout << host() << "> onReadBlock" << std::endl;
            onReadBlock(response);
        }
    });
}

SunSpecClient::~SunSpecClient() {
    //std::cout << "sunspecThing destroyed: " << host() << std::endl;
    _modbusClient->reset();
}

const std::string& SunSpecClient::host() const {
    return _modbusClient->ip();
}

uint8_t SunSpecClient::unitId() const {
    return _unitId;
}

const std::string& SunSpecClient::sunSpecId() const {
    return _sunSpecId;
}

std::string SunSpecClient::manufacturer() const {
    return _manufacturer;
}

std::string SunSpecClient::product() const {
    return _product;
}

std::string SunSpecClient::serial() const {
    return _serial;
}

const std::map<uint16_t, std::pair<uint16_t, uint16_t>>& SunSpecClient::models() const {
    return _blockAddresses;
}

bool SunSpecClient::readModel(uint16_t modelId) {
    //LOG_S(2) << this->sunSpecId() << "> reading model: " << modelId;
    if (_blockAddresses.count(modelId)) {
        readBlock(modelId, _blockAddresses[modelId].first, _blockAddresses[modelId].second);
        return true;
    }
    return false;
}

void SunSpecClient::doRead() {
    for (const auto& kv : _blockAddresses) {
        switch (kv.first) {
        case SunSpecModel::Id::InverterSinglePhase:
        case SunSpecModel::Id::InverterThreePhase:
        case SunSpecModel::Id::InverterSplitPhase:
        case SunSpecModel::Id::MeterWyeConnectThreePhase:
            readModel(kv.first);
            break;
        default:
            break;
        }
    }
}

uint8_t SunSpecClient::nextUnitId() const {
    static const std::vector<uint8_t> ids = {
        //// Primary Devices
        /// Requirement: detect first instance for each device group (for inverters detect first two).
        // Common
        //0,                // 0 is broadcast id. Do not check
        1,                  // Always check unit id 1 first
        // Smart Meters
        240,                // Fronius
        // Solar Inverters
        126, 127,           // SMA
        100,                // Fronius
        2,                  // Solaredge
        247,                // Goodwe

        //// Secondary Devices
        /// Requirement: detect next instance for each device group (for inverters detect next two).
        // Common
        // Smart Meters
        241,                // Fronius
        // Solar Inverters
        128, 129,           // SMA
        3,                  // Fronius
        4,                  // Solaredge

        //// Tertiary Devices
        /// Detect next three instances for each device group. Four for Inverters.
        // Common
        5,
        // Smart Meters
        242, 243, 244,                  // Fronius
        // Solar Inverters
        130, 131, 132, 133, 134, 135,   // SMA
        6, 7, 8, 9,                     // Fronius
        10,                             // Solaredge

        //// The Rest
         11,  12,  13,  14,  15,  16,  17,  18,  19,  20,
         21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
         31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
         41,  42,  43,  44,  45,  46,  47,  48,  49,  50,
         51,  52,  53,  54,  55,  56,  57,  58,  59,  60,
         61,  62,  63,  64,  65,  66,  67,  68,  69,  70,
         71,  72,  73,  74,  75,  76,  77,  78,  79,  80,
         81,  82,  83,  84,  85,  86,  87,  88,  89,  90,
         91,  92,  93,  94,  95,  96,  97,  98,  99,
        101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
        111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
        121, 122, 123, 124, 125,
                                 136, 137, 138, 139, 140,
        141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
        151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
        161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
        171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
        181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
        191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
        201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
        211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
        221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
        231, 232, 233, 234, 235, 236, 237, 238, 239,
                            245, 246
    };

    // unit ids: 1 to 247
    if (_unitIdx >= ids.size()) {
        return 0;
    }

    _unitId = ids.at(_unitIdx++);
    return _unitId;
}

void SunSpecClient::probeNextUnitId() {
    const uint8_t id = nextUnitId();
    if (id == 0) {
        //LOG_S(INFO) << host() << "> all ids checked. No SunSpec device found";
        // In the context of SunSpec, a close_event just means that the provided
        // ModbusThing does not provide a valid SunSpec header OR polling unitIds
        // finished without success.
        publish(uvw::close_event());
        return;
    }

    readHeader(id);
}

void SunSpecClient::readHeader(uint8_t id) {
    // Read entry point of sunspec.
    // First 2 registers form the well-known value 'SunS'(0x53756E53).
    // Next 2 registers if the common model (1) followed by its size (65 or 66).
    _modbusClient->readHoldingRegisters(id, 40000, 4, ReadHeader);
}

void SunSpecClient::onReadHeader(const ModbusResponse& response) {
    //DLOG_S(INFO) << host() << "> header received for unitId: " << (int)response.unitId;

    // For debugging only
    if (response.values.size() < 4) {
        //LOG_S(WARNING) << host() << "> header response too small: " << response.values.size();
        //pollNextUnitId();
        return;
    }

    _headerLength = response.values[3];
    if (response.values[0] == 0x5375 && response.values[1] == 0x6E53 &&
            response.values[2] == 1 && (_headerLength == 65 || _headerLength == 66)) {
        //publish(SunSpecUnitId{response.unitId, host()});
        readBlock(1, 40004, _headerLength);
    } else {
        // In the context of SunSpec, a close_event just means that the provided
        // ModbusThing does not provide a valid SunSpec header OR polling unitIds
        // finished without success.
        publish(uvw::close_event());
    }
}

void SunSpecClient::readModelEntry(uint16_t address) {
    _modbusClient->readHoldingRegisters(_unitId, address, 2, address);
}

void SunSpecClient::onReadModelEntry(const ModbusResponse& response) {
    // Check if we finished reading model table
    if (response.values[0] != 0xFFFF) {
        addModelAddress(response.values[0], response.userData + 2, response.values[1]);
        readModelEntry(response.userData + 2 + response.values[1]);
    } else {
        // In the context of SunSpecThing, a connect_event means, that we have identified
        // the provided ModBusThing as a SunSpecThing. Now read the models.
        publish(uvw::connect_event());
    }
}

void SunSpecClient::addModelAddress(uint16_t modelId, uint16_t startAddress, uint16_t length) {
    // Special handling for blocks, that do not fit into a 256 bytes payload (modbus tcp limit).
    //if (modelId == sunspec::Model::Id::InverterMpptExtension && length > 125) {
    //    length = 110;
    //}

    //LOG_S(1) << "model: " << modelId << ", address: " << startAddress << ", length: " << length;
    // Special handling for SMA solar inverters: we only request first two MPPs
    if (modelId == SunSpecModel::Id::InverterMpptExtension) {
        length = std::min(length, (uint16_t)48);
    }
    _blockAddresses[modelId] = { startAddress, length };
}

void SunSpecClient::readBlock(uint16_t modelId, uint16_t address, uint16_t length) {
    // Length == 2 means we are reading the model table.
    if (length == 2) {
        //LOG_S(WARNING) << "> not reading block with size 2";
        return;
    }
    _modbusClient->readHoldingRegisters(_unitId, address, length, modelId);
    //LOG_S(2) << sunSpecId() << "> reading block at: " << address;
}

void SunSpecClient::onReadBlock(const ModbusResponse& response) {
    parseBlock(response.userData, response.values);
    _timeoutCount = 0;

    // Read next model entry
    if (response.userData == 1)
        readModelEntry(40004 + _headerLength);
}

void SunSpecClient::parseBlock(uint16_t modelId, const std::vector<uint16_t>& buffer) {
    if (modelId == 1) {
        auto& commonModel = _models[modelId];
        if (SunSpecCommonModelFactory::updateFromBuffer(commonModel, buffer)) {
            _manufacturer = toString(commonModel.values().at(Manufacturer));
            _product = toString(commonModel.values().at(Product));
            _serial = toString(commonModel.values().at(Serial));
            _sunSpecId = _manufacturer + "_" + _product + "_" + _serial;
        }
    } else if (sunspec::ModelFactory::updateFromBuffer(_models, modelId, buffer)) {
        publish(_models.at(modelId));
    }
}

std::string SunSpecClient::toString(const LiveValue& v) {
    std::stringstream ss;
    ss << v;
    auto str = ss.str();
    std::transform(str.begin(), str.end(), str.begin(), [](char c) { return std::tolower(c); });
    str = std::regex_replace(str, std::regex("[^a-z0-9]"), "_");
    return str;
}

} // namespace sunspec
} // namespace uvw_net
