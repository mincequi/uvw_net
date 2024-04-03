#include <benchmark/benchmark.h>

#include <tao/json.hpp>

#include <iostream>

#include "BenchmarkMemoryManager.h"
#include "json.h"
#include "nadeau.h"

static void readJson(benchmark::State& state) {
    for (auto _ : state) {
        const tao::json::value v = tao::json::from_string(jsonStr);
    }
}

static void writeCbor(benchmark::State& state) {
    const tao::json::value v = tao::json::from_string(jsonStr);
    for (auto _ : state) {
        auto buffer = tao::json::cbor::to_string(v);
    }
}

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

BENCHMARK(readJson);
BENCHMARK(writeCbor);
//BENCHMARK_MAIN();
int main(int argc, char** argv) {
    ::benchmark::RegisterMemoryManager(&mm);
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::RegisterMemoryManager(nullptr);
}
