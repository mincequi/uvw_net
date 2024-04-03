add_library(http STATIC
  HttpClient.cpp
  HttpRequest.cpp
  HttpResponseParser.cpp
  HttpSession.cpp
)
add_library(uvw_net::http ALIAS http)

target_link_libraries(http
PUBLIC
  common
PRIVATE
  llhttp_static
)

target_include_directories(http
PUBLIC
  ../../
PRIVATE
  ${lhttp_SOURCE_DIR}/include
)
