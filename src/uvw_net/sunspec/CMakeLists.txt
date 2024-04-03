add_library(sunspec STATIC
  SunSpecBlock.cpp
  SunSpecClient.cpp
  SunSpecDataPoint.cpp
  SunSpecDiscovery.cpp
  SunSpecLogger.cpp
  SunSpecMeasuredValue.cpp
  SunSpecModel.cpp
  SunSpecModelFactory.cpp
  SunSpecStatsModel.cpp
  SunSpecStatsValue.cpp
  SunSpecTypes.cpp
  models/SunSpecCommonModelFactory.cpp
  models/SunSpecElgrisSmartMeterModelFactory.cpp
  models/SunSpecInverterModelFactory.cpp
  models/SunSpecMpptInverterExtensionModelFactory.cpp
  models/SunSpecWyeConnectMeterModelFactory.cpp
)
add_library(uvw_net::sunspec ALIAS sunspec)

target_link_libraries(sunspec
PUBLIC
  modbus
)

target_include_directories(sunspec
PUBLIC
  ../../
)
