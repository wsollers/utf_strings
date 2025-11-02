
#include <benchmark/benchmark.h>

#include <string>

#include "utf/utf_strings.hpp"

#ifdef HAVE_GPERFTOOLS
#include <gperftools/profiler.h>
#endif

static void BM_Length_Mixed(benchmark::State& state) {
  std::u8string s;
  for (int i = 0; i < 1000; ++i) s += u8"Héllø 🌍";
  for (auto _ : state) {
    auto n = utf::length<char8_t, utf::endian::big>(s);
    benchmark::DoNotOptimize(n);
  }
  state.SetComplexityN(static_cast<benchmark::ComplexityN>(s.size()));
  state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(s.size()));
}
BENCHMARK(BM_Length_Mixed)->Complexity();

int main(int argc, char** argv) {
#ifdef HAVE_GPERFTOOLS
  ProfilerStart("utf_strings.prof");
#endif
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
#ifdef HAVE_GPERFTOOLS
  ProfilerStop();
#endif
  return 0;
}
