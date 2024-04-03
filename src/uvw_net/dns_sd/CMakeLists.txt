add_library(dns_sd STATIC
  DnsMessage.cpp
  DnsParser.cpp
  DnsRecordDataSrv.cpp
  DnsServiceDiscovery.cpp
)
add_library(uvw_net::dns_sd ALIAS dns_sd)

target_link_libraries(dns_sd
PUBLIC
  common
)

target_include_directories(dns_sd
PUBLIC
  ../../
)
