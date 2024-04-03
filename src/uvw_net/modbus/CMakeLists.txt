add_library(modbus STATIC
  ModbusClient.cpp
  ModbusDiscovery.cpp
  ModbusRequest.cpp
  ModbusResponse.cpp
)
add_library(uvw_net::modbus ALIAS modbus)

target_link_libraries(modbus
PUBLIC
  common
)

target_include_directories(modbus
PUBLIC
  ../../
)
