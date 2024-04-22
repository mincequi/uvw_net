add_library(uvw_net_common STATIC
  Util.cpp
)
add_library(uvw_net::common ALIAS uvw_net_common)

target_link_libraries(uvw_net_common
PUBLIC
  uvw::uvw-static
  magic_enum::magic_enum
)

target_include_directories(uvw_net_common
PUBLIC
  ../../
)
