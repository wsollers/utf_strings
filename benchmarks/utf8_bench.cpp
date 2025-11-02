
/*
 * Copyright (c) 2025 William Sollers
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <benchmark/benchmark.h>

#include <string>

#include "utf/utf_strings.hpp"

#ifdef HAVE_GPERFTOOLS
#include <gperftools/profiler.h>
#endif

static void BM_CodePoint_Creation(benchmark::State& state) {
  // Benchmark UTF-8 code point creation from scalar values
  uint32_t scalars[] = {0x48, 0x00E9, 0x00F8, 0x1F30D};  // H, é, ø, 🌍
  std::size_t idx = 0;

  for (auto _ : state) {
    auto cp = utf::Utf8CodePoint::from_scalar(scalars[idx % 4]);
    benchmark::DoNotOptimize(cp);
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_CodePoint_Creation);

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
