add_library(uvw_net_sunspec STATIC
  SunSpecBlock.cpp
  SunSpecClient.cpp
  SunSpecDataPoint.cpp
  SunSpecDiscovery.cpp
  SunSpecLogger.cpp
  SunSpecMeasuredValue.cpp
  SunSpecModel.cpp
  SunSpecModelFactory.cpp
  SunSpecTypes.cpp
  models/SunSpecCommonModelFactory.cpp
  models/SunSpecElgrisSmartMeterModelFactory.cpp
  models/SunSpecInverterModelFactory.cpp
  models/SunSpecMpptInverterExtensionModelFactory.cpp
  models/SunSpecWyeConnectMeterModelFactory.cpp
)
add_library(uvw_net::sunspec ALIAS uvw_net_sunspec)

target_link_libraries(uvw_net_sunspec
PUBLIC
  uvw_net::modbus
)

target_include_directories(uvw_net_sunspec
PUBLIC
  ../../
)
