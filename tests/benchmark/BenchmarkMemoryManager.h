#pragma once

#include <benchmark/benchmark.h>

class BenchmarkMemoryManager : public benchmark::MemoryManager {
public:
    void Start() override {
        num_allocs = 0;
        max_bytes_used = 0;
    }
    void Stop(benchmark::MemoryManager::Result& result) override {
        result.num_allocs = num_allocs;
        result.max_bytes_used = max_bytes_used;
    }

    int64_t num_allocs;
    int64_t max_bytes_used;
};
