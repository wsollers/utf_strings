#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#include "utf/utf_strings.hpp"

// Fuzz target for UTF-32 Big Endian validation and parsing
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < 4 || size % 4 != 0) return 0;  // Need multiples of 4 bytes for UTF-32

  try {
    // Create UTF-32 string from raw data (interpret as big-endian)
    std::u32string input;
    input.reserve(size / 4);
    for (size_t i = 0; i < size; i += 4) {
      uint32_t unit =
          (static_cast<uint32_t>(data[i]) << 24) | (static_cast<uint32_t>(data[i + 1]) << 16) |
          (static_cast<uint32_t>(data[i + 2]) << 8) | static_cast<uint32_t>(data[i + 3]);
      input.push_back(static_cast<char32_t>(unit));
    }

    // Test UTF-32 big endian
    utf::utf32be_string utf32_str = utf::utf32be_string::from_native(input);

    // Test validation
    bool is_valid = utf32_str.valid();

    // Test length calculation
    auto length_opt = utf32_str.length();

    // Test conversion to UTF-32 (should be identity for valid strings)
    auto u32_opt = utf32_str.to_u32();

    // Test spans calculation
    auto spans_opt = utf32_str.spans();

    // Test view operations
    auto view = utf32_str.view();
    auto str_ref = utf32_str.str();

    // Test native conversion
    auto native = utf32_str.to_native();

    // Test free functions
    bool valid_view = utf::valid<char32_t, utf::endian::big>(view);
    auto length_view = utf::length<char32_t, utf::endian::big>(view);
    auto u32_view = utf::to_u32<char32_t, utf::endian::big>(view);

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

      // Verify UTF-32 length matches spans count and input size (1:1 for UTF-32)
      if (u32_opt->size() != spans.size() || u32_opt->size() != input.size()) {
        std::abort();  // UTF-32 length should match span count and input size
      }

      // For UTF-32, each span should have unit_length = 1
      for (const auto& span : spans) {
        if (span.unit_length != 1) {
          std::abort();  // UTF-32 spans should always have length 1
        }
      }

      // Test round-trip conversion consistency
      if (native != input) {
        std::abort();  // Round-trip conversion failed
      }

      // UTF-32 to UTF-32 conversion should be identity
      if (*u32_opt != std::u32string(input.begin(), input.end())) {
        std::abort();  // UTF-32 to UTF-32 should be identity
      }
    } else {
      // If invalid, these should return nullopt
      if (length_opt.has_value() || u32_opt.has_value() || spans_opt.has_value()) {
        std::abort();  // Should be nullopt for invalid strings
      }
    }

    // Test for invalid code points
    for (size_t i = 0; i < input.size(); ++i) {
      uint32_t unit = static_cast<uint32_t>(input[i]);
      if (unit > 0x10FFFF || (unit >= 0xD800 && unit <= 0xDFFF)) {
        // Invalid code point - string should be invalid
        if (is_valid) {
          std::abort();  // Should be invalid
        }
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