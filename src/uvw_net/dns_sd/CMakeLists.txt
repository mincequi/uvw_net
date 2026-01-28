add_library(uvw_net_dns_sd STATIC
  DnsMessage.cpp
  DnsParser.cpp
  DnsRecordDataSrv.cpp
  DnsServiceDiscovery.cpp
  DnsOpcode.h
  DnsRcode.h
  DnsResourceRecord.h
  MdnsResponse.h
)
add_library(uvw_net::dns_sd ALIAS uvw_net_dns_sd)

target_link_libraries(uvw_net_dns_sd
PUBLIC
  uvw_net::common
)

target_include_directories(uvw_net_dns_sd
PUBLIC
  ../../
)
