
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
#include <vector>

#include "utf/utf_codepoints.hpp"

#ifdef HAVE_GPERFTOOLS
#include <gperftools/profiler.h>
#endif

// Test data: ASCII, 2-byte, 3-byte, and 4-byte UTF-8 characters
static const uint32_t test_scalars[] = {
    0x48,     // H (ASCII, 1 byte)
    0x00E9,   // é (2 bytes)
    0x00F8,   // ø (2 bytes)
    0x20AC,   // € (3 bytes)
    0x1F30D,  // 🌍 (4 bytes)
    0x1F680,  // 🚀 (4 bytes)
    0x1F4A9,  // 💩 (4 bytes)
    0x65      // e (ASCII, 1 byte)
};

static void BM_UTF8_CodePoint_Creation(benchmark::State& state) {
  // Benchmark UTF-8 code point creation from scalar values
  std::size_t idx = 0;
  const auto scalar_count = sizeof(test_scalars) / sizeof(test_scalars[0]);

  for (auto _ : state) {
    auto cp = utf::Utf8CodePoint::from_scalar(test_scalars[idx % scalar_count]);
    benchmark::DoNotOptimize(cp);
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_CodePoint_Creation);

static void BM_UTF16BE_CodePoint_Creation(benchmark::State& state) {
  // Benchmark UTF-16 BE code point creation from scalar values
  std::size_t idx = 0;
  const auto scalar_count = sizeof(test_scalars) / sizeof(test_scalars[0]);

  for (auto _ : state) {
    auto cp = utf::Utf16BECodePoint::from_scalar(test_scalars[idx % scalar_count]);
    benchmark::DoNotOptimize(cp);
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF16BE_CodePoint_Creation);

static void BM_UTF32LE_CodePoint_Creation(benchmark::State& state) {
  // Benchmark UTF-32 LE code point creation from scalar values
  std::size_t idx = 0;
  const auto scalar_count = sizeof(test_scalars) / sizeof(test_scalars[0]);

  for (auto _ : state) {
    auto cp = utf::Utf32LECodePoint::from_scalar(test_scalars[idx % scalar_count]);
    benchmark::DoNotOptimize(cp);
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF32LE_CodePoint_Creation);

static void BM_UTF8_Scalar_Conversion(benchmark::State& state) {
  // Benchmark converting UTF-8 code points back to scalar values
  std::vector<utf::Utf8CodePoint> codepoints;
  const auto scalar_count = sizeof(test_scalars) / sizeof(test_scalars[0]);

  // Pre-create code points
  for (std::size_t i = 0; i < scalar_count; ++i) {
    auto cp = utf::Utf8CodePoint::from_scalar(test_scalars[i]);
    if (cp.has_value()) {
      codepoints.push_back(*cp);
    }
  }

  std::size_t idx = 0;
  for (auto _ : state) {
    if (!codepoints.empty()) {
      auto scalar = codepoints[idx % codepoints.size()].to_scalar();
      benchmark::DoNotOptimize(scalar);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_Scalar_Conversion);

static void BM_UTF8_Validation(benchmark::State& state) {
  // Benchmark UTF-8 code point validation
  std::vector<utf::Utf8CodePoint> codepoints;
  const auto scalar_count = sizeof(test_scalars) / sizeof(test_scalars[0]);

  // Pre-create code points
  for (std::size_t i = 0; i < scalar_count; ++i) {
    auto cp = utf::Utf8CodePoint::from_scalar(test_scalars[i]);
    if (cp.has_value()) {
      codepoints.push_back(*cp);
    }
  }

  std::size_t idx = 0;
  for (auto _ : state) {
    if (!codepoints.empty()) {
      bool valid = codepoints[idx % codepoints.size()].is_valid();
      benchmark::DoNotOptimize(valid);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_Validation);

static void BM_UTF8_to_UTF16BE_Conversion(benchmark::State& state) {
  // Benchmark conversion from UTF-8 to UTF-16 BE
  std::vector<utf::Utf8CodePoint> utf8_codepoints;
  const auto scalar_count = sizeof(test_scalars) / sizeof(test_scalars[0]);

  // Pre-create UTF-8 code points
  for (std::size_t i = 0; i < scalar_count; ++i) {
    auto cp = utf::Utf8CodePoint::from_scalar(test_scalars[i]);
    if (cp.has_value()) {
      utf8_codepoints.push_back(*cp);
    }
  }

  std::size_t idx = 0;
  for (auto _ : state) {
    if (!utf8_codepoints.empty()) {
      auto utf16be_cp =
          utf::convert<utf::Utf16BECodePoint>(utf8_codepoints[idx % utf8_codepoints.size()]);
      benchmark::DoNotOptimize(utf16be_cp);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_to_UTF16BE_Conversion);

static void BM_UTF16BE_to_UTF32LE_Conversion(benchmark::State& state) {
  // Benchmark conversion from UTF-16 BE to UTF-32 LE
  std::vector<utf::Utf16BECodePoint> utf16be_codepoints;
  const auto scalar_count = sizeof(test_scalars) / sizeof(test_scalars[0]);

  // Pre-create UTF-16 BE code points
  for (std::size_t i = 0; i < scalar_count; ++i) {
    auto cp = utf::Utf16BECodePoint::from_scalar(test_scalars[i]);
    if (cp.has_value()) {
      utf16be_codepoints.push_back(*cp);
    }
  }

  std::size_t idx = 0;
  for (auto _ : state) {
    if (!utf16be_codepoints.empty()) {
      auto utf32le_cp =
          utf::convert<utf::Utf32LECodePoint>(utf16be_codepoints[idx % utf16be_codepoints.size()]);
      benchmark::DoNotOptimize(utf32le_cp);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF16BE_to_UTF32LE_Conversion);

static void BM_UTF8_Units_Access(benchmark::State& state) {
  // Benchmark accessing UTF-8 code point units/bytes
  std::vector<utf::Utf8CodePoint> codepoints;
  const auto scalar_count = sizeof(test_scalars) / sizeof(test_scalars[0]);

  // Pre-create code points
  for (std::size_t i = 0; i < scalar_count; ++i) {
    auto cp = utf::Utf8CodePoint::from_scalar(test_scalars[i]);
    if (cp.has_value()) {
      codepoints.push_back(*cp);
    }
  }

  std::size_t idx = 0;
  for (auto _ : state) {
    if (!codepoints.empty()) {
      const auto& cp = codepoints[idx % codepoints.size()];
      auto units = cp.units();
      auto count = cp.count();
      benchmark::DoNotOptimize(units);
      benchmark::DoNotOptimize(count);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_Units_Access);

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
