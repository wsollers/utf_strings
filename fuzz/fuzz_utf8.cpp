#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "utf/utf_strings.hpp"

// Fuzz target for UTF-8 validation and parsing
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size == 0) return 0;

  try {
    // Create UTF-8 string from raw data
    std::u8string input;
    input.reserve(size);
    for (size_t i = 0; i < size; ++i) {
      input.push_back(static_cast<char8_t>(data[i]));
    }

    // Test UTF-8 big endian (endian is ignored for UTF-8)
    utf::utf8_string utf8_str{input};

    // Test validation
    bool is_valid = utf8_str.valid();

    // Test length calculation
    auto length_opt = utf8_str.length();

    // Test conversion to UTF-32
    auto u32_opt = utf8_str.to_u32();

    // Test spans calculation
    auto spans_opt = utf8_str.spans();

    // Test view operations
    auto view = utf8_str.view();
    auto str_ref = utf8_str.str();

    // Test native conversion (no-op for UTF-8)
    auto native = utf8_str.to_native();

    // Test free functions
    bool valid_view = utf::valid<char8_t, utf::endian::big>(view);
    auto length_view = utf::length<char8_t, utf::endian::big>(view);
    auto u32_view = utf::to_u32<char8_t, utf::endian::big>(view);

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
      if (total_units != size) {
        std::abort();  // Spans don't add up to input size
      }

      // Verify UTF-32 length matches spans count
      if (u32_opt->size() != spans.size()) {
        std::abort();  // UTF-32 length doesn't match span count
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