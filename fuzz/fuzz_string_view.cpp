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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF POSSESSION OF SUCH DAMAGE.
 */

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <utf.hpp>
#include <vector>

// Fuzz target for UTF StringView operations
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size == 0) return 0;

  try {
    // Test UTF-8 StringView creation from raw data
    if (size <= 4096) {  // Limit size for reasonable performance
      const char* char_data = reinterpret_cast<const char*>(data);

      // Test different StringView construction methods
      const uint8_t* u8_data = reinterpret_cast<const uint8_t*>(char_data);
      utf::string::Utf8StringView view_from_ptr(u8_data, size);

      // Test basic properties
      auto length = view_from_ptr.length();
      auto byte_size = view_from_ptr.size();
      auto is_valid = view_from_ptr.is_valid();

      if (length != size) {
        std::abort();  // Length should match input size
      }

      if (byte_size != size) {
        std::abort();  // Size should match input size for UTF-8
      }

      // Test iteration and code point counting
      size_t iteration_count = 0;
      size_t code_point_count = 0;

      try {
        code_point_count = view_from_ptr.count_code_points();

        for (auto cp : view_from_ptr) {
          ++iteration_count;

          // Check that individual code points have reasonable properties
          auto cp_count = cp.count();
          if (cp_count > 4) {
            std::abort();  // UTF-8 code points cannot exceed 4 bytes
          }
        }

        if (is_valid && iteration_count != code_point_count) {
          std::abort();  // For valid strings, iteration should match count
        }

      } catch (...) {
        // count_code_points() or iteration might throw for invalid UTF-8
        // This is acceptable behavior
      }

      // Test copy operations
      utf::string::Utf8StringView view_copy = view_from_ptr;
      if (view_copy.data() != view_from_ptr.data() ||
          view_copy.length() != view_from_ptr.length()) {
        std::abort();  // Copy should reference same data
      }

      // Test equality operations
      if (!(view_from_ptr == view_copy)) {
        std::abort();  // Identical views should be equal
      }

      // Test substring operations if not empty
      if (size > 0) {
        // Test first byte
        auto first_byte_view = utf::string::Utf8StringView(u8_data, 1);
        if (first_byte_view.length() != 1) {
          std::abort();  // Single byte view should have length 1
        }

        // Test last byte if size > 1
        if (size > 1) {
          auto last_byte_view = utf::string::Utf8StringView(u8_data + size - 1, 1);
          if (last_byte_view.length() != 1) {
            std::abort();  // Single byte view should have length 1
          }
        }
      }

      // Test std::string construction if reasonable size
      if (size <= 256) {
        std::basic_string<uint8_t> std_str(u8_data, u8_data + size);
        utf::string::Utf8StringView view_from_std(std_str);

        if (view_from_std.length() != size || view_from_std.size() != size) {
          std::abort();  // StringView from std::string should match size
        }
      }
    }

    // Test UTF-16 StringView if we have even byte count
    if (size >= 2 && size % 2 == 0) {
      const uint16_t* u16_data = reinterpret_cast<const uint16_t*>(data);
      size_t u16_count = size / 2;

      // Test UTF-16 BE StringView
      utf::string::Utf16BEStringView u16be_view(u16_data, u16_count);

      auto u16be_length = u16be_view.length();
      auto u16be_size = u16be_view.size();

      if (u16be_length != u16_count) {
        std::abort();  // Length should match unit count
      }

      if (u16be_size != size) {
        std::abort();  // Size should match byte count
      }

      // Test UTF-16 LE StringView
      utf::string::Utf16LEStringView u16le_view(u16_data, u16_count);

      if (u16le_view.length() != u16_count || u16le_view.size() != size) {
        std::abort();  // LE view should have same size properties as BE
      }

      // Test iteration for small inputs
      if (u16_count <= 32) {
        try {
          size_t be_iteration_count = 0;
          size_t le_iteration_count = 0;

          for (auto cp : u16be_view) {
            ++be_iteration_count;
          }

          for (auto cp : u16le_view) {
            ++le_iteration_count;
          }

          // Both endianness views should iterate over same number of code points
          // if the data represents valid UTF-16 in either endianness
          // (This is not always true, but we're testing iteration consistency)

        } catch (...) {
          // Iteration might fail for invalid UTF-16, which is acceptable
        }
      }
    }

    // Test UTF-32 StringView if we have proper alignment
    if (size >= 4 && size % 4 == 0) {
      const uint32_t* u32_data = reinterpret_cast<const uint32_t*>(data);
      size_t u32_count = size / 4;

      if (u32_count <= 16) {  // Limit for performance
        // Test UTF-32 BE StringView
        utf::string::Utf32BEStringView u32be_view(u32_data, u32_count);

        if (u32be_view.length() != u32_count || u32be_view.size() != size) {
          std::abort();  // UTF-32 view size should match input
        }

        // Test UTF-32 LE StringView
        utf::string::Utf32LEStringView u32le_view(u32_data, u32_count);

        if (u32le_view.length() != u32_count || u32le_view.size() != size) {
          std::abort();  // UTF-32 LE view should have same size as BE
        }

        // For UTF-32, each unit is one code point, so iteration should match unit count
        try {
          size_t be_cp_count = u32be_view.count_code_points();
          size_t le_cp_count = u32le_view.count_code_points();

          // If valid, UTF-32 code point count should equal unit count
          if (u32be_view.is_valid() && be_cp_count != u32_count) {
            std::abort();  // UTF-32 code point count should equal unit count
          }

          if (u32le_view.is_valid() && le_cp_count != u32_count) {
            std::abort();  // UTF-32 code point count should equal unit count
          }

        } catch (...) {
          // count_code_points might fail for invalid UTF-32
        }
      }
    }

    // Test empty StringView operations
    const uint8_t empty_data[] = "";
    utf::string::Utf8StringView empty_view(empty_data, 0);
    if (empty_view.length() != 0 || empty_view.size() != 0 || !empty_view.is_valid() ||
        empty_view.count_code_points() != 0) {
      std::abort();  // Empty view should have zero size and be valid
    }

    // Test null pointer edge case
    utf::string::Utf8StringView null_view(nullptr, 0);
    if (null_view.length() != 0 || null_view.size() != 0) {
      std::abort();  // Null view with zero length should have zero size
    }

  } catch (const std::exception& e) {
    // StringView operations should generally not throw, but some validation might
  } catch (...) {
    // Unexpected exceptions should not occur
    std::abort();
  }

  return 0;
}