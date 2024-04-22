add_executable(uvw_net_tests
  common/UtilTest.cpp
  dns_sd/DnsMessageTest.cpp
  dns_sd/DnsParserTest.cpp
  dns_sd/DnsRecordDataSrvTest.cpp
  dns_sd/DnsServiceDiscoveryTest.cpp
  http/HttpClientTest.cpp
  http/HttpRequestTest.cpp
  http/HttpResponseParserTest.cpp
  http/HttpSessionTest.cpp
  modbus/ModbusClientTest.cpp
  modbus/ModbusDiscoveryTest.cpp
  modbus/ModbusRequestTest.cpp
  modbus/ModbusResponseTest.cpp
)

target_link_libraries(uvw_net_tests
PRIVATE
  uvw_net::common
  uvw_net::dns_sd
  uvw_net::http
  uvw_net::modbus
  Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(uvw_net_tests)
