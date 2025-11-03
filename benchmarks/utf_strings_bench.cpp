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

#include <array>
#include <cstring>
#include <string>
#include <utf.hpp>
#include <vector>

// Test data: Various UTF-8 strings of different lengths and complexities
static const char* utf8_test_strings[] = {
    "Hello",                                   // ASCII only
    "Hello, 世界!",                            // Mixed ASCII + CJK
    "🌍🚀💩",                                  // Emoji only (all 4-byte)
    "café résumé naïve",                       // Latin with accents
    "Здравствуй мир",                          // Cyrillic
    "こんにちは世界",                          // Japanese Hiragana + Kanji
    "",                                        // Empty string
    "A",                                       // Single ASCII
    "€",                                       // Single 3-byte
    "𝕿𝖍𝖎𝖘 𝖎𝖘 𝖆 𝖑𝖔𝖓𝖌 𝖘𝖙𝖗𝖎𝖓𝖌 𝖜𝖎𝖙𝖍 𝖒𝖆𝖙𝖍 𝖘𝖞𝖒𝖇𝖔𝖑𝖘"  // Mathematical symbols
};

constexpr size_t num_test_strings = sizeof(utf8_test_strings) / sizeof(utf8_test_strings[0]);

// Prepare test byte arrays for factory benchmarks
static std::vector<std::vector<uint8_t>> utf8_byte_arrays;
static std::vector<std::vector<uint8_t>> utf16_be_byte_arrays;
static std::vector<std::vector<uint8_t>> utf32_le_byte_arrays;

// Initialize test data (called once)
static void InitializeTestData() {
  static bool initialized = false;
  if (initialized) return;

  for (size_t i = 0; i < num_test_strings; ++i) {
    const char* str = utf8_test_strings[i];
    size_t len = std::strlen(str);

    // UTF-8 byte arrays
    utf8_byte_arrays.emplace_back(reinterpret_cast<const uint8_t*>(str),
                                  reinterpret_cast<const uint8_t*>(str) + len);

    // Create UTF-16 BE and UTF-32 LE equivalents by converting through CodePoints
    std::vector<uint8_t> utf16_bytes, utf32_bytes;

    try {
      auto utf8_str = utf::string::Utf8String::from_bytes(utf8_byte_arrays.back());
      if (utf8_str.has_value()) {
        // Convert to UTF-16 BE
        auto utf16_str = utf::string::to_utf16_be_string(utf8_str->view());
        if (utf16_str.has_value()) {
          auto std_u16 = utf16_str->to_std_string();
          const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(std_u16.data());
          utf16_bytes.assign(byte_ptr, byte_ptr + std_u16.size() * 2);
        }

        // Convert to UTF-32 LE
        auto utf32_str = utf::string::to_utf32_le_string(utf8_str->view());
        if (utf32_str.has_value()) {
          auto std_u32 = utf32_str->to_std_string();
          const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(std_u32.data());
          utf32_bytes.assign(byte_ptr, byte_ptr + std_u32.size() * 4);
        }
      }
    } catch (...) {
      // If conversion fails, use empty arrays
    }

    utf16_be_byte_arrays.emplace_back(std::move(utf16_bytes));
    utf32_le_byte_arrays.emplace_back(std::move(utf32_bytes));
  }

  initialized = true;
}

// ============================================================================
// String Factory Method Benchmarks
// ============================================================================

static void BM_UTF8_String_FromBytes(benchmark::State& state) {
  InitializeTestData();
  size_t idx = 0;

  for (auto _ : state) {
    const auto& bytes = utf8_byte_arrays[idx % num_test_strings];
    auto str_opt = utf::string::Utf8String::from_bytes(bytes);
    benchmark::DoNotOptimize(str_opt);
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_String_FromBytes);

static void BM_UTF16BE_String_FromBytes(benchmark::State& state) {
  InitializeTestData();
  size_t idx = 0;

  for (auto _ : state) {
    const auto& bytes = utf16_be_byte_arrays[idx % num_test_strings];
    if (!bytes.empty()) {
      auto str_opt = utf::string::Utf16BEString::from_bytes(bytes);
      benchmark::DoNotOptimize(str_opt);
    }
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF16BE_String_FromBytes);

static void BM_UTF32LE_String_FromBytes(benchmark::State& state) {
  InitializeTestData();
  size_t idx = 0;

  for (auto _ : state) {
    const auto& bytes = utf32_le_byte_arrays[idx % num_test_strings];
    if (!bytes.empty()) {
      auto str_opt = utf::string::Utf32LEString::from_bytes(bytes);
      benchmark::DoNotOptimize(str_opt);
    }
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF32LE_String_FromBytes);

// ============================================================================
// String Construction Benchmarks
// ============================================================================

static void BM_UTF8_String_FromCString(benchmark::State& state) {
  size_t idx = 0;

  for (auto _ : state) {
    const char* cstr = utf8_test_strings[idx % num_test_strings];
    const uint8_t* u8_cstr = reinterpret_cast<const uint8_t*>(cstr);
    utf::string::Utf8String str{u8_cstr};
    benchmark::DoNotOptimize(str);
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_String_FromCString);

static void BM_UTF8_String_CopyConstruction(benchmark::State& state) {
  InitializeTestData();

  // Pre-create strings
  std::vector<utf::string::Utf8String> strings;
  for (size_t i = 0; i < num_test_strings; ++i) {
    auto str_opt = utf::string::Utf8String::from_bytes(utf8_byte_arrays[i]);
    if (str_opt.has_value()) {
      strings.emplace_back(std::move(*str_opt));
    }
  }

  size_t idx = 0;
  for (auto _ : state) {
    if (!strings.empty()) {
      utf::string::Utf8String copy{strings[idx % strings.size()]};
      benchmark::DoNotOptimize(copy);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_String_CopyConstruction);

static void BM_UTF8_String_Assignment(benchmark::State& state) {
  InitializeTestData();

  // Pre-create strings
  std::vector<utf::string::Utf8String> strings;
  for (size_t i = 0; i < num_test_strings; ++i) {
    auto str_opt = utf::string::Utf8String::from_bytes(utf8_byte_arrays[i]);
    if (str_opt.has_value()) {
      strings.emplace_back(std::move(*str_opt));
    }
  }

  utf::string::Utf8String target;
  size_t idx = 0;

  for (auto _ : state) {
    if (!strings.empty()) {
      target = strings[idx % strings.size()];
      benchmark::DoNotOptimize(target);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_String_Assignment);

// ============================================================================
// String Conversion Benchmarks
// ============================================================================

static void BM_UTF8_to_UTF16BE_String_Conversion(benchmark::State& state) {
  InitializeTestData();

  // Pre-create UTF-8 strings
  std::vector<utf::string::Utf8String> utf8_strings;
  for (size_t i = 0; i < num_test_strings; ++i) {
    auto str_opt = utf::string::Utf8String::from_bytes(utf8_byte_arrays[i]);
    if (str_opt.has_value()) {
      utf8_strings.emplace_back(std::move(*str_opt));
    }
  }

  size_t idx = 0;
  for (auto _ : state) {
    if (!utf8_strings.empty()) {
      const auto& utf8_str = utf8_strings[idx % utf8_strings.size()];
      auto utf16_opt = utf::string::to_utf16_be_string(utf8_str.view());
      benchmark::DoNotOptimize(utf16_opt);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_to_UTF16BE_String_Conversion);

static void BM_UTF16BE_to_UTF32LE_String_Conversion(benchmark::State& state) {
  InitializeTestData();

  // Pre-create UTF-16 BE strings
  std::vector<utf::string::Utf16BEString> utf16_strings;
  for (size_t i = 0; i < num_test_strings; ++i) {
    if (!utf16_be_byte_arrays[i].empty()) {
      auto str_opt = utf::string::Utf16BEString::from_bytes(utf16_be_byte_arrays[i]);
      if (str_opt.has_value()) {
        utf16_strings.emplace_back(std::move(*str_opt));
      }
    }
  }

  size_t idx = 0;
  for (auto _ : state) {
    if (!utf16_strings.empty()) {
      const auto& utf16_str = utf16_strings[idx % utf16_strings.size()];
      auto utf32_opt = utf::string::to_utf32_le_string(utf16_str.view());
      benchmark::DoNotOptimize(utf32_opt);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF16BE_to_UTF32LE_String_Conversion);

static void BM_UTF8_String_RoundTrip_Conversion(benchmark::State& state) {
  InitializeTestData();

  // Pre-create UTF-8 strings
  std::vector<utf::string::Utf8String> utf8_strings;
  for (size_t i = 0; i < num_test_strings; ++i) {
    auto str_opt = utf::string::Utf8String::from_bytes(utf8_byte_arrays[i]);
    if (str_opt.has_value()) {
      utf8_strings.emplace_back(std::move(*str_opt));
    }
  }

  size_t idx = 0;
  for (auto _ : state) {
    if (!utf8_strings.empty()) {
      const auto& utf8_str = utf8_strings[idx % utf8_strings.size()];

      // UTF-8 -> UTF-16 BE -> UTF-8
      auto utf16_opt = utf::string::to_utf16_be_string(utf8_str.view());
      if (utf16_opt.has_value()) {
        auto back_to_utf8_opt = utf::string::to_utf8_string(utf16_opt->view());
        benchmark::DoNotOptimize(back_to_utf8_opt);
      }
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_String_RoundTrip_Conversion);

// ============================================================================
// String Concatenation Benchmarks
// ============================================================================

static void BM_UTF8_String_Concatenation(benchmark::State& state) {
  InitializeTestData();

  // Pre-create UTF-8 strings
  std::vector<utf::string::Utf8String> utf8_strings;
  for (size_t i = 0; i < num_test_strings; ++i) {
    auto str_opt = utf::string::Utf8String::from_bytes(utf8_byte_arrays[i]);
    if (str_opt.has_value()) {
      utf8_strings.emplace_back(std::move(*str_opt));
    }
  }

  size_t idx = 0;
  for (auto _ : state) {
    if (utf8_strings.size() >= 2) {
      const auto& str1 = utf8_strings[idx % utf8_strings.size()];
      const auto& str2 = utf8_strings[(idx + 1) % utf8_strings.size()];
      auto result = str1 + str2;
      benchmark::DoNotOptimize(result);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_String_Concatenation);

static void BM_UTF8_String_Append_Operator(benchmark::State& state) {
  InitializeTestData();

  // Pre-create UTF-8 strings
  std::vector<utf::string::Utf8String> utf8_strings;
  for (size_t i = 0; i < num_test_strings; ++i) {
    auto str_opt = utf::string::Utf8String::from_bytes(utf8_byte_arrays[i]);
    if (str_opt.has_value()) {
      utf8_strings.emplace_back(std::move(*str_opt));
    }
  }

  size_t idx = 0;
  for (auto _ : state) {
    if (utf8_strings.size() >= 2) {
      auto result = utf8_strings[idx % utf8_strings.size()];
      result += utf8_strings[(idx + 1) % utf8_strings.size()];
      benchmark::DoNotOptimize(result);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_String_Append_Operator);

// ============================================================================
// String Iteration and Counting Benchmarks
// ============================================================================

static void BM_UTF8_String_CodePoint_Counting(benchmark::State& state) {
  InitializeTestData();

  // Pre-create UTF-8 strings
  std::vector<utf::string::Utf8String> utf8_strings;
  for (size_t i = 0; i < num_test_strings; ++i) {
    auto str_opt = utf::string::Utf8String::from_bytes(utf8_byte_arrays[i]);
    if (str_opt.has_value()) {
      utf8_strings.emplace_back(std::move(*str_opt));
    }
  }

  size_t idx = 0;
  for (auto _ : state) {
    if (!utf8_strings.empty()) {
      const auto& str = utf8_strings[idx % utf8_strings.size()];
      auto count = str.count_code_points();
      benchmark::DoNotOptimize(count);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_String_CodePoint_Counting);

static void BM_UTF8_String_Iteration(benchmark::State& state) {
  InitializeTestData();

  // Pre-create UTF-8 strings
  std::vector<utf::string::Utf8String> utf8_strings;
  for (size_t i = 0; i < num_test_strings; ++i) {
    auto str_opt = utf::string::Utf8String::from_bytes(utf8_byte_arrays[i]);
    if (str_opt.has_value()) {
      utf8_strings.emplace_back(std::move(*str_opt));
    }
  }

  size_t idx = 0;
  for (auto _ : state) {
    if (!utf8_strings.empty()) {
      const auto& str = utf8_strings[idx % utf8_strings.size()];
      size_t count = 0;
      for (auto cp : str) {
        benchmark::DoNotOptimize(cp);
        ++count;
      }
      benchmark::DoNotOptimize(count);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_String_Iteration);

static void BM_UTF8_String_Validation(benchmark::State& state) {
  InitializeTestData();

  // Pre-create UTF-8 strings
  std::vector<utf::string::Utf8String> utf8_strings;
  for (size_t i = 0; i < num_test_strings; ++i) {
    auto str_opt = utf::string::Utf8String::from_bytes(utf8_byte_arrays[i]);
    if (str_opt.has_value()) {
      utf8_strings.emplace_back(std::move(*str_opt));
    }
  }

  size_t idx = 0;
  for (auto _ : state) {
    if (!utf8_strings.empty()) {
      const auto& str = utf8_strings[idx % utf8_strings.size()];
      bool valid = str.is_valid();
      benchmark::DoNotOptimize(valid);
      ++idx;
    }
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_String_Validation);

// ============================================================================
// StringView Benchmarks
// ============================================================================

static void BM_UTF8_StringView_Creation(benchmark::State& state) {
  size_t idx = 0;

  for (auto _ : state) {
    const char* cstr = utf8_test_strings[idx % num_test_strings];
    size_t len = std::strlen(cstr);
    const uint8_t* u8_cstr = reinterpret_cast<const uint8_t*>(cstr);
    utf::string::Utf8StringView view{u8_cstr, len};
    benchmark::DoNotOptimize(view);
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_StringView_Creation);

static void BM_UTF8_StringView_CodePoint_Counting(benchmark::State& state) {
  // Pre-create string views
  std::vector<utf::string::Utf8StringView> views;
  for (size_t i = 0; i < num_test_strings; ++i) {
    const char* str = utf8_test_strings[i];
    size_t len = std::strlen(str);
    const uint8_t* u8_str = reinterpret_cast<const uint8_t*>(str);
    views.emplace_back(u8_str, len);
  }

  size_t idx = 0;
  for (auto _ : state) {
    const auto& view = views[idx % views.size()];
    auto count = view.count_code_points();
    benchmark::DoNotOptimize(count);
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_StringView_CodePoint_Counting);

static void BM_UTF8_StringView_Iteration(benchmark::State& state) {
  // Pre-create string views
  std::vector<utf::string::Utf8StringView> views;
  for (size_t i = 0; i < num_test_strings; ++i) {
    const char* str = utf8_test_strings[i];
    size_t len = std::strlen(str);
    const uint8_t* u8_str = reinterpret_cast<const uint8_t*>(str);
    views.emplace_back(u8_str, len);
  }

  size_t idx = 0;
  for (auto _ : state) {
    const auto& view = views[idx % views.size()];
    size_t count = 0;
    for (auto cp : view) {
      benchmark::DoNotOptimize(cp);
      ++count;
    }
    benchmark::DoNotOptimize(count);
    ++idx;
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_UTF8_StringView_Iteration);