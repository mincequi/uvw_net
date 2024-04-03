add_executable(discover
  discover.cpp
)
target_link_libraries(discover
  common
  dns_sd
  sunspec
)

add_executable(http_get
  http_get.cpp
)
target_link_libraries(http_get
  common
  http
)
