include(../../cmake/CPM.cmake)

add_compile_definitions(MEMORY_PROFILER)

CPMAddPackage(
  NAME benchmark
  GITHUB_REPOSITORY google/benchmark
  VERSION 1.8.3
  OPTIONS "BENCHMARK_ENABLE_TESTING Off"
)

### TAOCPP
CPMAddPackage("gh:taocpp/json#1.0.0-beta.14")
add_executable(taocpp
  taocpp.cpp
)
target_link_libraries(taocpp
  taocpp-json
  benchmark
)

### NLOHMANN
CPMAddPackage(
    NAME nlohmann_json
    GITHUB_REPOSITORY nlohmann/json
    VERSION 3.11.3
)
add_executable(nlohmann_
  nlohmann.cpp
)
target_link_libraries(nlohmann_
  nlohmann_json
  benchmark
)

### JSONCONS
CPMAddPackage("gh:danielaparker/jsoncons@0.173.4")
add_executable(jsoncons_
  jsoncons.cpp
)
target_link_libraries(jsoncons_
  jsoncons
  benchmark
)

### GOLDFISH
#CPMAddPackage("gh:fschoenm/GoldFish#ca0af57")
#CPMAddPackage("gh:fschoenm/GoldFish#edcd488") # This one is sane
CPMAddPackage("gh:mincequi/GoldFish#fd4d302")
add_executable(goldfish
  goldfish.cpp
)
target_link_libraries(goldfish
  benchmark
)
target_include_directories(goldfish
PRIVATE
  ${GoldFish_SOURCE_DIR}/inc
)
