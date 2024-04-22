CPMAddPackage(gh:catchorg/Catch2@3.5.4)
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

add_subdirectory(uvw_net)

# Benchmark
option(BUILD_BENCHMARK "Build benchmark programs" OFF)
if(BUILD_BENCHMARK)
  add_subdirectory(benchmark)
endif()
