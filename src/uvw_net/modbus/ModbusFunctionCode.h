#pragma once

#include <cstdint>

namespace uvw_net {
namespace modbus {

enum class ModbusFunctionCode : uint8_t {
    ReadCoils = 0x01,              // Function code 0x01: Read Coils
    ReadDiscreteInputs = 0x02,     // Function code 0x02: Read Discrete Inputs
    ReadHoldingRegisters = 0x03,   // Function code 0x03: Read Holding Registers
    ReadInputRegisters = 0x04,     // Function code 0x04: Read Input Registers
    WriteSingleCoil = 0x05,        // Function code 0x05: Write Single Coil
    WriteSingleRegister = 0x06,    // Function code 0x06: Write Single Register
    WriteMultipleCoils = 0x0F,     // Function code 0x0F: Write Multiple Coils
    WriteMultipleRegisters = 0x10, // Function code 0x10: Write Multiple Registers
    ReportSlaveID = 0x11,          // Function code 0x11: Report Slave ID
    MaskWriteRegister = 0x16,      // Function code 0x16: Mask Write Register
    ReadWriteMultipleRegisters = 0x17 // Function code 0x17: Read/Write Multiple Registers
};

} // namespace modbus
} // namespace uvw_net
