#include <benchmark/benchmark.h>

#include <goldfish/cbor_writer.h>
#include <goldfish/json_reader.h>

#include <iostream>

#include "BenchmarkMemoryManager.h"
#include "json.h"
#include "nadeau.h"

using namespace goldfish;

static void readJson(benchmark::State& state) {
    for (auto _ : state) {
        auto doc = json::read(stream::read_string(jsonStr));
    }
}

static void writeCbor(benchmark::State& state) {
    auto doc = json::read(stream::read_string(jsonStr));
    for (auto _ : state) {
        auto cbor_document = cbor::create_writer(stream::vector_writer{}).write(doc);
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
