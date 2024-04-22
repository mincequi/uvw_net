#include "BenchmarkMemoryManager.h"
BenchmarkMemoryManager mm;

#ifdef MEMORY_PROFILER
void *custom_malloc(size_t size) {
    void *p = malloc(size);
    mm.num_allocs += 1;
    mm.max_bytes_used += size;
    return p;
}
#define malloc(size) custom_malloc(size)
void* operator new(size_t size) {
    mm.num_allocs += 1;
    mm.max_bytes_used += size;
    return malloc(size);
}
#endif

#include <benchmark/benchmark.h>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/cbor/cbor.hpp>

#include <iostream>

#include "json.h"
#include "nadeau.h"

using namespace jsoncons;

static void readJson(benchmark::State& state) {
    for (auto _ : state) {
        auto doc = json::parse(jsonStr);
    }
}

static void writeCbor(benchmark::State& state) {
    auto doc = json::parse(jsonStr);

    for (auto _ : state) {
        std::vector<uint8_t> buffer;
        cbor::encode_cbor(doc, buffer);
    }
}

BENCHMARK(readJson);
BENCHMARK(writeCbor);
//BENCHMARK_MAIN();
int main(int argc, char** argv) {
    ::benchmark::RegisterMemoryManager(&mm);
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::RegisterMemoryManager(nullptr);
}
