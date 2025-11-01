#include <benchmark/benchmark.h>
#include <string>
#include "utf/utf_strings.hpp"
static void BM_Len(benchmark::State& s){ std::u8string t; for(int i=0;i<1000;++i) t+=u8"Héllø 🌍"; for(auto _ : s){ auto n=utf::length<char8_t, utf::endian::big>(t); benchmark::DoNotOptimize(n);}}
BENCHMARK(BM_Len);
BENCHMARK_MAIN();
