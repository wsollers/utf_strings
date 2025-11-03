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
#include <iostream>
#include <string>
#include <vector>

#include "utf/utf_codepoints.hpp"

// Fuzz target for UTF-32 Big Endian CodePoint validation and parsing
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size == 0) return 0;

  try {
    // Test scalar-based code point creation from fuzz input
    std::vector<utf::Utf32BECodePoint> valid_codepoints;
    std::vector<uint32_t> test_scalars;

    // Generate test scalars from input data
    for (size_t i = 0; i + 3 < size; i += 4) {
      uint32_t scalar =
          (static_cast<uint32_t>(data[i]) << 24) | (static_cast<uint32_t>(data[i + 1]) << 16) |
          (static_cast<uint32_t>(data[i + 2]) << 8) | static_cast<uint32_t>(data[i + 3]);
      test_scalars.push_back(scalar);
    }

    // Also test smaller scalars
    for (size_t i = 0; i < size; ++i) {
      test_scalars.push_back(static_cast<uint32_t>(data[i]));

      if (i + 1 < size) {
        uint32_t two_byte =
            (static_cast<uint32_t>(data[i]) << 8) | static_cast<uint32_t>(data[i + 1]);
        test_scalars.push_back(two_byte);
      }

      if (i + 2 < size) {
        uint32_t three_byte = (static_cast<uint32_t>(data[i]) << 16) |
                              (static_cast<uint32_t>(data[i + 1]) << 8) |
                              static_cast<uint32_t>(data[i + 2]);
        test_scalars.push_back(three_byte);
      }
    }

    // Test each potential scalar with UTF-32 BE
    for (uint32_t scalar : test_scalars) {
      auto cp_opt = utf::Utf32BECodePoint::from_scalar(scalar);

      if (cp_opt.has_value()) {
        const auto& cp = *cp_opt;
        valid_codepoints.push_back(cp);

        // Validate the code point
        if (!cp.is_valid()) {
          std::abort();
        }

        // Test scalar round-trip
        auto result_scalar_opt = cp.to_scalar();
        if (!result_scalar_opt.has_value()) {
          std::abort();
        }

        uint32_t result_scalar = *result_scalar_opt;
        if (scalar <= 0x10FFFF && !(scalar >= 0xD800 && scalar <= 0xDFFF)) {
          if (result_scalar != scalar) {
            std::abort();
          }
        }

        // Test unit consistency (UTF-32 always uses 1 unit)
        size_t count = cp.count();
        if (count != 1) {
          std::abort();
        }

        if (cp.size() != 4) {  // UTF-32 is always 4 bytes
          std::abort();
        }

        auto units = cp.units();
        if (units.size() != 1) {
          std::abort();
        }

        // Test conversions to other encodings
        auto utf8_opt = utf::convert<utf::Utf8CodePoint>(cp);
        if (utf8_opt.has_value() && utf8_opt->to_scalar_unchecked() != result_scalar) {
          std::abort();
        }

        auto utf16le_opt = utf::convert<utf::Utf16LECodePoint>(cp);
        if (utf16le_opt.has_value() && utf16le_opt->to_scalar_unchecked() != result_scalar) {
          std::abort();
        }
      }
    }
  } catch (const std::exception& e) {
    // CodePoint operations should not throw exceptions, only return nullopt
    std::abort();
  } catch (...) {
    // No exceptions should be thrown
    std::abort();
  }

  return 0;
}