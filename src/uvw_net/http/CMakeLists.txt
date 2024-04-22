add_library(uvw_net_http STATIC
  HttpClient.cpp
  HttpRequest.cpp
  HttpResponseParser.cpp
  HttpSession.cpp
)
add_library(uvw_net::http ALIAS uvw_net_http)

target_link_libraries(uvw_net_http
PUBLIC
  uvw_net::common
PRIVATE
  llhttp_static
)

target_include_directories(uvw_net_http
PUBLIC
  ../../
PRIVATE
  ${lhttp_SOURCE_DIR}/include
)
