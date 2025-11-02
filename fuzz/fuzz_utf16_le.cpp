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

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#include "utf/utf_strings.hpp"

// Fuzz target for UTF-16 Little Endian validation and parsing
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < 2 || size % 2 != 0) return 0;  // Need even number of bytes for UTF-16

  try {
    // Create UTF-16 string from raw data (interpret as little-endian)
    std::u16string input;
    input.reserve(size / 2);
    for (size_t i = 0; i < size; i += 2) {
      uint16_t unit = static_cast<uint16_t>(data[i]) | (static_cast<uint16_t>(data[i + 1]) << 8);
      input.push_back(static_cast<char16_t>(unit));
    }

    // Test UTF-16 little endian
    utf::utf16le_string utf16_str = utf::utf16le_string::from_native(input);

    // Test validation
    bool is_valid = utf16_str.valid();

    // Test length calculation
    auto length_opt = utf16_str.length();

    // Test conversion to UTF-32
    auto u32_opt = utf16_str.to_u32();

    // Test spans calculation
    auto spans_opt = utf16_str.spans();

    // Test view operations
    auto view = utf16_str.view();
    auto str_ref = utf16_str.str();

    // Test native conversion
    auto native = utf16_str.to_native();

    // Test free functions
    bool valid_view = utf::valid<char16_t, utf::endian::little>(view);
    auto length_view = utf::length<char16_t, utf::endian::little>(view);
    auto u32_view = utf::to_u32<char16_t, utf::endian::little>(view);

    // Consistency checks
    if (is_valid) {
      // If valid, length should be available
      if (!length_opt.has_value()) {
        std::abort();  // Inconsistent state
      }

      // If valid, UTF-32 conversion should work
      if (!u32_opt.has_value()) {
        std::abort();  // Inconsistent state
      }

      // If valid, spans should be available
      if (!spans_opt.has_value()) {
        std::abort();  // Inconsistent state
      }

      // View operations should be consistent
      if (valid_view != is_valid) {
        std::abort();  // Inconsistent validation
      }

      if (length_view != length_opt) {
        std::abort();  // Inconsistent length
      }

      // Verify spans consistency
      const auto& spans = *spans_opt;
      size_t total_units = 0;
      for (const auto& span : spans) {
        total_units += span.unit_length;
      }
      if (total_units != input.size()) {
        std::abort();  // Spans don't add up to input size
      }

      // Verify UTF-32 length matches spans count
      if (u32_opt->size() != spans.size()) {
        std::abort();  // UTF-32 length doesn't match span count
      }

      // Test round-trip conversion consistency
      if (native != input) {
        std::abort();  // Round-trip conversion failed
      }
    } else {
      // If invalid, these should return nullopt
      if (length_opt.has_value() || u32_opt.has_value() || spans_opt.has_value()) {
        std::abort();  // Should be nullopt for invalid strings
      }
    }

  } catch (const std::exception& e) {
    // UTF operations should not throw exceptions, only return nullopt
    std::abort();
  } catch (...) {
    // No exceptions should be thrown
    std::abort();
  }

  return 0;
}