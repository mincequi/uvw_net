add_library(uvw_net_modbus STATIC
  ModbusClient.cpp
  ModbusDiscovery.cpp
  ModbusRequest.cpp
  ModbusResponse.cpp
  ModbusUtil.cpp
  ModbusFunctionCode.h
)
add_library(uvw_net::modbus ALIAS uvw_net_modbus)

target_link_libraries(uvw_net_modbus
PUBLIC
  uvw_net::common
)

target_include_directories(uvw_net_modbus
PUBLIC
  ../../
)
