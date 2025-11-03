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

// Fuzz target for UTF-8 String operations and factories
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size == 0) return 0;

  try {
    // Test UTF-8 string creation from byte arrays with fuzz input
    auto utf8_string_opt = utf::string::Utf8String::from_bytes(data, size);

    // Test different byte array sizes and alignments
    if (size >= 4) {
      // Test creating from partial data
      auto partial_string_opt = utf::string::Utf8String::from_bytes(data, size / 2);

      // Test factory function variant
      auto factory_string_opt = utf::string::utf8_string_from_bytes(data, size);

      // Consistency check: if both succeed, they should be equal
      if (utf8_string_opt.has_value() && factory_string_opt.has_value()) {
        if (utf8_string_opt->view() != factory_string_opt->view()) {
          std::abort();  // Factory methods should be consistent
        }
      }
    }

    // Test vector-based creation
    std::vector<uint8_t> byte_vector(data, data + size);
    auto vector_string_opt = utf::string::Utf8String::from_bytes(byte_vector);
    auto vector_factory_opt = utf::string::utf8_string_from_bytes(byte_vector);

    // Consistency check for vector methods
    if (vector_string_opt.has_value() && vector_factory_opt.has_value()) {
      if (vector_string_opt->view() != vector_factory_opt->view()) {
        std::abort();  // Vector and pointer methods should be consistent
      }
    }

    // If we successfully created a string, test its operations
    if (utf8_string_opt.has_value()) {
      const auto& utf8_str = *utf8_string_opt;

      // Test basic properties
      auto length = utf8_str.length();
      auto byte_size = utf8_str.size();
      auto code_point_count = utf8_str.count_code_points();
      auto is_valid = utf8_str.is_valid();

      // A successfully created string should be valid
      if (!is_valid) {
        std::abort();  // from_bytes should only create valid strings
      }

      // Test view conversion
      auto view = utf8_str.view();
      if (view.length() != length || view.size() != byte_size) {
        std::abort();  // View should match string properties
      }

      // Test conversion to std::string
      auto std_string = utf8_str.to_std_string();
      if (std_string.size() != byte_size) {
        std::abort();  // std::string size should match byte size
      }

      // Test iteration
      size_t iteration_count = 0;
      for (auto cp : utf8_str) {
        if (!cp.is_valid()) {
          std::abort();  // All code points in valid string should be valid
        }
        ++iteration_count;
      }

      if (iteration_count != code_point_count) {
        std::abort();  // Iteration count should match code point count
      }

      // Test copy constructor and assignment
      utf::string::Utf8String copy_constructed{utf8_str};
      if (copy_constructed.view() != utf8_str.view()) {
        std::abort();  // Copy constructor should create identical string
      }

      utf::string::Utf8String copy_assigned;
      copy_assigned = utf8_str;
      if (copy_assigned.view() != utf8_str.view()) {
        std::abort();  // Assignment should create identical string
      }

      // Test string concatenation (limit size to avoid excessive memory usage)
      if (size <= 1024) {
        utf::string::Utf8String concatenated = utf8_str + utf8_str;
        if (concatenated.count_code_points() != 2 * code_point_count) {
          std::abort();  // Concatenation should double code point count
        }

        // Test append operations
        utf::string::Utf8String appended{utf8_str};
        appended += utf8_str;
        if (appended.view() != concatenated.view()) {
          std::abort();  // Append and concatenation should be equivalent
        }
      }

      // Test string conversions to other encodings (limit to avoid expensive operations)
      if (code_point_count <= 32) {
        // Convert to UTF-16 BE
        auto utf16be_opt = utf::string::to_utf16_be_string(utf8_str.view());
        if (utf16be_opt.has_value()) {
          if (utf16be_opt->count_code_points() != code_point_count) {
            std::abort();  // Code point count should be preserved in conversion
          }

          // Convert back to UTF-8
          auto back_to_utf8 = utf::string::to_utf8_string(utf16be_opt->view());
          if (back_to_utf8.has_value()) {
            if (back_to_utf8->view() != utf8_str.view()) {
              std::abort();  // Round-trip conversion should preserve content
            }
          }
        }

        // Convert to UTF-32 LE
        auto utf32le_opt = utf::string::to_utf32_le_string(utf8_str.view());
        if (utf32le_opt.has_value()) {
          if (utf32le_opt->count_code_points() != code_point_count) {
            std::abort();  // Code point count should be preserved in conversion
          }

          // Convert back to UTF-8
          auto back_to_utf8 = utf::string::to_utf8_string(utf32le_opt->view());
          if (back_to_utf8.has_value()) {
            if (back_to_utf8->view() != utf8_str.view()) {
              std::abort();  // Round-trip conversion should preserve content
            }
          }
        }
      }

      // Test individual code point operations if string is not too long
      if (code_point_count <= 16 && code_point_count > 0) {
        // Build string from individual code points
        utf::string::Utf8String rebuilt;
        for (auto cp : utf8_str) {
          rebuilt += cp;
        }

        if (rebuilt.view() != utf8_str.view()) {
          std::abort();  // Rebuilding from code points should preserve string
        }
      }
    }

    // Test empty string operations
    utf::string::Utf8String empty_string;
    if (!empty_string.is_valid() || empty_string.length() != 0 ||
        empty_string.count_code_points() != 0) {
      std::abort();  // Empty string should be valid with zero length
    }

    // Test string construction from literal if data looks like a C string
    if (size > 0 && data[size - 1] == 0) {
      // Appears to be null-terminated, test C-string construction
      utf::string::Utf8String from_cstr{data};

      // Test that it matches byte array construction of the same data
      auto from_bytes_cstr =
          utf::string::Utf8String::from_bytes(data, size - 1);  // Exclude null terminator
      if (from_bytes_cstr.has_value()) {
        if (from_cstr.view() != from_bytes_cstr->view()) {
          std::abort();  // C-string and byte array construction should match
        }
      }
    }

    // Test boundary conditions with small inputs
    if (size >= 1) {
      // Single byte
      auto single_byte = utf::string::Utf8String::from_bytes(data, 1);
      if (single_byte.has_value() && single_byte->count_code_points() > 1) {
        std::abort();  // Single UTF-8 byte cannot contain multiple code points
      }
    }

    if (size >= 2) {
      // Two bytes
      auto two_bytes = utf::string::Utf8String::from_bytes(data, 2);
      if (two_bytes.has_value() && two_bytes->count_code_points() > 2) {
        std::abort();  // Two UTF-8 bytes cannot contain more than 2 code points
      }
    }

  } catch (const std::exception& e) {
    // String operations should generally not throw, only return nullopt
    // Exception might be okay for some operations like invalid conversion assignment
    // But fuzzing should not cause crashes
  } catch (...) {
    // Unexpected exceptions should not occur during fuzzing
    std::abort();
  }

  return 0;
}