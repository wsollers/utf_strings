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
#include <utf.hpp>
#include <vector>

// Fuzz target for UTF-8 CodePoint validation and parsing
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size == 0) return 0;

  try {
    // Test scalar-based code point creation from fuzz input
    std::vector<utf::Utf8CodePoint> valid_codepoints;
    std::vector<uint32_t> test_scalars;

    // Generate test scalars from input data
    for (size_t i = 0; i + 3 < size; i += 4) {
      uint32_t scalar =
          (static_cast<uint32_t>(data[i]) << 24) | (static_cast<uint32_t>(data[i + 1]) << 16) |
          (static_cast<uint32_t>(data[i + 2]) << 8) | static_cast<uint32_t>(data[i + 3]);
      test_scalars.push_back(scalar);
    }

    // Also test smaller scalars for single, double, triple byte inputs
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

    // Test each potential scalar
    for (uint32_t scalar : test_scalars) {
      auto cp_opt = utf::Utf8CodePoint::from_scalar(scalar);

      if (cp_opt.has_value()) {
        const auto& cp = *cp_opt;
        valid_codepoints.push_back(cp);

        // If we got a code point, it must be valid
        if (!cp.is_valid()) {
          std::abort();  // from_scalar should only return valid code points
        }

        // Test scalar round-trip
        auto result_scalar_opt = cp.to_scalar();
        if (!result_scalar_opt.has_value()) {
          std::abort();  // Valid code point should have valid scalar
        }

        uint32_t result_scalar = *result_scalar_opt;

        // For valid Unicode scalars, the result should match
        if (scalar <= 0x10FFFF && !(scalar >= 0xD800 && scalar <= 0xDFFF)) {
          if (result_scalar != scalar) {
            std::abort();  // Scalar round-trip mismatch
          }
        }

        // Test unchecked scalar matches checked version
        uint32_t unchecked_scalar = cp.to_scalar_unchecked();
        if (unchecked_scalar != result_scalar) {
          std::abort();  // Checked and unchecked scalar mismatch
        }

        // Test byte count consistency
        size_t count = cp.count();
        if (count == 0 || count > 4) {
          std::abort();  // Invalid UTF-8 byte count
        }

        // Test size consistency (for UTF-8, size == count)
        if (cp.size() != count) {
          std::abort();  // Size should equal count for UTF-8
        }

        // Test units span consistency
        auto units = cp.units();
        if (units.size() != count) {
          std::abort();  // Units size should match count
        }

        // Validate UTF-8 encoding rules
        const uint8_t* bytes = cp.data();
        if (count == 1) {
          // ASCII: 0xxxxxxx
          if (bytes[0] >= 0x80) {
            std::abort();  // Invalid 1-byte UTF-8
          }
        } else if (count == 2) {
          // 110xxxxx 10xxxxxx
          if ((bytes[0] & 0xE0) != 0xC0 || (bytes[1] & 0xC0) != 0x80) {
            std::abort();  // Invalid 2-byte UTF-8
          }
        } else if (count == 3) {
          // 1110xxxx 10xxxxxx 10xxxxxx
          if ((bytes[0] & 0xF0) != 0xE0 || (bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80) {
            std::abort();  // Invalid 3-byte UTF-8
          }
        } else if (count == 4) {
          // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
          if ((bytes[0] & 0xF8) != 0xF0 || (bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80 ||
              (bytes[3] & 0xC0) != 0x80) {
            std::abort();  // Invalid 4-byte UTF-8
          }
        }
      }
    }

    // Test conversions between encodings for first few valid code points
    size_t conversion_limit = std::min(valid_codepoints.size(), size_t(5));
    for (size_t i = 0; i < conversion_limit; ++i) {
      const auto& utf8_cp = valid_codepoints[i];

      // Convert to UTF-16 BE
      auto utf16be_opt = utf::convert<utf::Utf16BECodePoint>(utf8_cp);
      if (utf16be_opt.has_value()) {
        if (!utf16be_opt->is_valid()) {
          std::abort();  // Converted code point should be valid
        }

        auto utf8_scalar = utf8_cp.to_scalar_unchecked();
        auto utf16be_scalar = utf16be_opt->to_scalar_unchecked();
        if (utf16be_scalar != utf8_scalar) {
          std::abort();  // Scalar should be preserved in conversion
        }

        // Convert back to UTF-8
        auto back_to_utf8 = utf::convert<utf::Utf8CodePoint>(*utf16be_opt);
        if (back_to_utf8.has_value()) {
          if (back_to_utf8->to_scalar_unchecked() != utf8_scalar) {
            std::abort();  // Round-trip conversion failed
          }
        }
      }

      // Convert to UTF-32 LE
      auto utf32le_opt = utf::convert<utf::Utf32LECodePoint>(utf8_cp);
      if (utf32le_opt.has_value()) {
        if (!utf32le_opt->is_valid()) {
          std::abort();  // Converted code point should be valid
        }

        auto utf8_scalar = utf8_cp.to_scalar_unchecked();
        auto utf32le_scalar = utf32le_opt->to_scalar_unchecked();
        if (utf32le_scalar != utf8_scalar) {
          std::abort();  // Scalar should be preserved in conversion
        }

        // Convert back to UTF-8
        auto back_to_utf8 = utf::convert<utf::Utf8CodePoint>(*utf32le_opt);
        if (back_to_utf8.has_value()) {
          if (back_to_utf8->to_scalar_unchecked() != utf8_scalar) {
            std::abort();  // Round-trip conversion failed
          }
        }
      }
    }

    // Test known invalid scalar ranges
    if (size >= 1) {
      uint32_t invalid_base = 0xD800 + (data[0] % 0x800);  // Surrogate range
      auto invalid_cp = utf::Utf8CodePoint::from_scalar(invalid_base);
      if (invalid_cp.has_value()) {
        std::abort();  // Should not create code point from surrogate
      }

      if (size >= 2) {
        uint32_t too_large = 0x110000 + (static_cast<uint32_t>(data[0]) << 8) + data[1];
        auto large_cp = utf::Utf8CodePoint::from_scalar(too_large);
        if (large_cp.has_value()) {
          std::abort();  // Should not create code point beyond Unicode range
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