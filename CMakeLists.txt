cmake_minimum_required(VERSION 3.14)

project(uvw_net LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# add dependencies
include(cmake/CPM.cmake)

set(BUILD_UVW_LIBS ON)
CPMAddPackage(gh:skypjack/uvw@3.3.0_libuv_v1.47)
CPMAddPackage(gh:Neargye/magic_enum@0.9.5)
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "")
CPMAddPackage(
  NAME llhttp
  URL https://github.com/nodejs/llhttp/archive/refs/tags/release/v9.2.0.tar.gz
)

add_subdirectory(src/uvw_net/common)
add_subdirectory(src/uvw_net/dns_sd)
add_subdirectory(src/uvw_net/http)
add_subdirectory(src/uvw_net/modbus)
add_subdirectory(src/uvw_net/sunspec)

# Tests
option(BUILD_TESTING "Build test programs" OFF)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

# Tools
option(BUILD_TOOLS "Build tool programs" OFF)
if(BUILD_TOOLS)
  add_subdirectory(tools)
endif()
