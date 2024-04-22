add_library(common STATIC
  Util.cpp
)
add_library(uvw_net::common ALIAS common)

target_link_libraries(common
PUBLIC
  uvw::uvw-static
  magic_enum::magic_enum
)

target_include_directories(common
PUBLIC
  ../../
)
