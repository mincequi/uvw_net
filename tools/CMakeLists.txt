add_executable(discover
  discover.cpp
)
target_link_libraries(discover
  uvw_net::common
  uvw_net::dns_sd
  uvw_net::sunspec
)

add_executable(http_get
  http_get.cpp
)
target_link_libraries(http_get
  uvw_net::common
  uvw_net::http
)

add_executable(mdns-discover
  mdns-discover.cpp
)
target_link_libraries(mdns-discover
  uvw_net::common
  uvw_net::dns_sd
)
