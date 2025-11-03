#pragma once

// utf_codepoint.hpp - Modern C++23 UTF Code Point Library
//
// A type-safe, constexpr-enabled library for handling UTF-8, UTF-16, and UTF-32
// code points with explicit endianness control.
//
// Features:
// - UTF-8/16/32 encoding and decoding
// - Explicit endianness control (Big Endian / Little Endian)
// - Compile-time validation via concepts
// - constexpr and noexcept throughout for zero runtime overhead
// - Safe construction via factory functions returning std::optional
// - Full validation including overlong encoding detection
// - Optimal memory layout: all CodePoint types are exactly 4 bytes
//
// Requirements:
// - C++23 or later
// - Standard library support for: <bit>, <span>, <concepts>, <optional>
//
// Example Usage:
//   // Create UTF-8 code point from Unicode scalar
//   auto cp = utf::Utf8CodePoint::from_scalar(0x1F4A9);  // 💩
//   if (cp) {
//       // Convert to UTF-16 Little Endian
//       auto u16 = utf::convert<utf::Utf16LECodePoint>(*cp);
//       if (u16) {
//           // Use the code point
//           auto scalar = u16->to_scalar();
//       }
//   }
//
//   // Fast path when input is known to be valid
//   utf::Utf8CodePoint valid_cp{0x41};  // 'A'
//   auto u32 = utf::convert_unchecked<utf::Utf32BECodePoint>(valid_cp);
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef UTF_CODEPOINT_HPP
#define UTF_CODEPOINT_HPP

#define UTF_CODEPOINT_VERSION_MAJOR 0
#define UTF_CODEPOINT_VERSION_MINOR 0
#define UTF_CODEPOINT_VERSION_PATCH 2

#include <array>
#include <bit>
#include <concepts>
#include <cstdint>
#include <optional>
#include <span>

namespace utf {

// ============================================================================
// Unicode Limits and Constants
// ============================================================================

/// @brief Unicode-related constants and limits
namespace limits {
/// Maximum valid Unicode code point (U+10FFFF)
constexpr uint32_t max_code_point = 0x10FFFF;

/// Sentinel value indicating an invalid Unicode scalar
constexpr uint32_t invalid_scalar = 0xFFFFFFFF;

/// Start of Unicode surrogate pair range (invalid as scalar values)
constexpr uint32_t surrogate_min = 0xD800;

/// End of Unicode surrogate pair range (invalid as scalar values)
constexpr uint32_t surrogate_max = 0xDFFF;

/// Start of UTF-16 high surrogate range
constexpr uint16_t high_surrogate_min = 0xD800;

/// End of UTF-16 high surrogate range
constexpr uint16_t high_surrogate_max = 0xDBFF;

/// Start of UTF-16 low surrogate range
constexpr uint16_t low_surrogate_min = 0xDC00;

/// End of UTF-16 low surrogate range
constexpr uint16_t low_surrogate_max = 0xDFFF;

/// Offset used in UTF-16 surrogate pair calculation
constexpr uint32_t surrogate_offset = 0x10000;

/// UTF-8 boundaries
constexpr uint32_t utf8_1byte_max = 0x7F;
constexpr uint32_t utf8_2byte_max = 0x7FF;
constexpr uint32_t utf8_3byte_max = 0xFFFF;
constexpr uint32_t utf8_4byte_max = 0x10FFFF;

/// Maximum code point in the Basic Multilingual Plane (BMP)
constexpr uint32_t bmp_max = 0xFFFF;
}  // namespace limits

// ============================================================================
// Error Codes
// ============================================================================

/// @brief Error codes for UTF operations
enum class ErrorCode {
  invalid_scalar,     ///< Unicode scalar value is invalid
  overlong_encoding,  ///< UTF-8 overlong encoding detected (security issue)
  invalid_surrogate,  ///< Invalid surrogate pair or unpaired surrogate
  out_of_range,       ///< Code point exceeds valid Unicode range
  truncated_sequence  ///< Incomplete UTF sequence
};

// ============================================================================
// Endianness
// ============================================================================

/// @brief Endianness-related types and constants
namespace endianness {
/// @brief Byte order specification
enum class Type {
  None,  ///< Byte-oriented encoding (no endianness applies, e.g., UTF-8)
  BE,    ///< Big Endian (network byte order)
  LE     ///< Little Endian
};

/// Convenience alias for byte-oriented encoding
inline constexpr Type none = Type::None;

/// Convenience alias for big endian
inline constexpr Type big_endian = Type::BE;

/// Convenience alias for little endian
inline constexpr Type little_endian = Type::LE;

/// Convenience alias for network byte order (same as big endian)
inline constexpr Type network_byte_order = Type::BE;
}  // namespace endianness

// Import endianness type into utf namespace for convenience
using Endian = endianness::Type;

// ============================================================================
// UTF Encodings
// ============================================================================

/// @brief UTF encoding type definitions
namespace encodings {
/// @brief UTF-8 encoding specification
struct Utf8 {
  using storage_type = uint8_t;
  static constexpr std::size_t unit_size = 1;
  static constexpr std::size_t max_units = 4;
};

/// @brief UTF-16 encoding specification
struct Utf16 {
  using storage_type = uint16_t;
  static constexpr std::size_t unit_size = 2;
  static constexpr std::size_t max_units = 2;
};

/// @brief UTF-32 encoding specification
struct Utf32 {
  using storage_type = uint32_t;
  static constexpr std::size_t unit_size = 4;
  static constexpr std::size_t max_units = 1;
};
}  // namespace encodings

// Import encoding types into utf namespace for convenience
using Utf8 = encodings::Utf8;
using Utf16 = encodings::Utf16;
using Utf32 = encodings::Utf32;

// ============================================================================
// Strong Type for Unicode Scalar
// ============================================================================

/// @brief Strong type wrapper for Unicode scalar values
/// @details Provides type safety to distinguish Unicode scalars from raw integers
struct UnicodeScalar {
  uint32_t value;  ///< The Unicode scalar value

  /// @brief Construct from a raw integer value
  constexpr explicit UnicodeScalar(uint32_t v) noexcept : value(v) {}

  /// @brief Check if this represents a valid Unicode scalar value
  /// @return true if the value is in the valid Unicode range and not a surrogate
  [[nodiscard]] constexpr bool is_valid() const noexcept {
    using namespace limits;
    return value <= max_code_point && !(value >= surrogate_min && value <= surrogate_max);
  }

  /// @brief Implicit conversion to uint32_t
  constexpr operator uint32_t() const noexcept { return value; }
};

// ============================================================================
// Concepts
// ============================================================================

/// @brief Concept for byte-oriented UTF encodings (UTF-8)
template <typename UtfType>
concept ByteOriented = std::same_as<UtfType, Utf8>;

/// @brief Concept for multi-byte UTF encodings (UTF-16, UTF-32)
template <typename UtfType>
concept MultiByteOriented = std::same_as<UtfType, Utf16> || std::same_as<UtfType, Utf32>;

/// @brief Concept validating endianness for a given encoding
/// @details UTF-8 must use Endian::None, UTF-16/32 must use BE or LE
template <typename UtfType, Endian E>
concept ValidEndianness = (ByteOriented<UtfType> && E == Endian::None) ||
                          (MultiByteOriented<UtfType> && E != Endian::None);

// Forward declaration
template <typename UtfType, Endian E = Endian::BE>
  requires ValidEndianness<UtfType, E>
struct CodePoint;

/// @brief Concept to check if a type is a valid CodePoint instantiation
template <typename T>
concept IsCodePoint = requires {
  typename T::encoding_type;
  { T::endianness } -> std::convertible_to<Endian>;
};

// ============================================================================
// UTF-8 CodePoint Specialization
// ============================================================================

/// @brief UTF-8 code point representation
/// @details Stores a single Unicode code point encoded as UTF-8 (1-4 bytes).
/// Total size: exactly 4 bytes (optimal alignment).
///
/// UTF-8 is byte-oriented so endianness does not apply.
/// Length is computed on-demand from the leading byte pattern.
///
/// @note Construction may create invalid code points. Always check is_valid()
/// after construction, or use from_scalar() factory function for safe construction.
/// Default construction creates U+0000 (null character), which is a valid code point.
template <Endian E>
  requires(ByteOriented<Utf8> && E == Endian::None)
struct CodePoint<Utf8, E> {
  using encoding_type = Utf8;
  static constexpr Endian endianness = E;

  std::array<uint8_t, 4> rune{};  ///< UTF-8 encoded bytes

  /// @brief Default constructor creates a null character (U+0000)
  /// @note U+0000 is a valid Unicode code point.
  constexpr CodePoint() noexcept = default;

  /// @brief Construct from a Unicode scalar value
  /// @param unicode_scalar The Unicode code point to encode (U+0000 to U+10FFFF)
  /// @note May create invalid CodePoint if scalar is out of range or a surrogate.
  ///       Always check is_valid() after construction.
  constexpr explicit CodePoint(uint32_t unicode_scalar) noexcept {
    using namespace limits;

    if (unicode_scalar <= utf8_1byte_max) {
      // 1-byte sequence: 0xxxxxxx
      rune[0] = static_cast<uint8_t>(unicode_scalar);
    } else if (unicode_scalar <= utf8_2byte_max) {
      // 2-byte sequence: 110xxxxx 10xxxxxx
      rune[0] = static_cast<uint8_t>(0xC0 | (unicode_scalar >> 6));
      rune[1] = static_cast<uint8_t>(0x80 | (unicode_scalar & 0x3F));
    } else if (unicode_scalar <= utf8_3byte_max) {
      // 3-byte sequence: 1110xxxx 10xxxxxx 10xxxxxx
      // Check for surrogate range (invalid)
      if (unicode_scalar >= surrogate_min && unicode_scalar <= surrogate_max) {
        // Mark as invalid by setting an invalid pattern
        rune[0] = 0xFF;  // 0xFF is never a valid UTF-8 lead byte
        return;
      }
      rune[0] = static_cast<uint8_t>(0xE0 | (unicode_scalar >> 12));
      rune[1] = static_cast<uint8_t>(0x80 | ((unicode_scalar >> 6) & 0x3F));
      rune[2] = static_cast<uint8_t>(0x80 | (unicode_scalar & 0x3F));
    } else if (unicode_scalar <= utf8_4byte_max) {
      // 4-byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
      rune[0] = static_cast<uint8_t>(0xF0 | (unicode_scalar >> 18));
      rune[1] = static_cast<uint8_t>(0x80 | ((unicode_scalar >> 12) & 0x3F));
      rune[2] = static_cast<uint8_t>(0x80 | ((unicode_scalar >> 6) & 0x3F));
      rune[3] = static_cast<uint8_t>(0x80 | (unicode_scalar & 0x3F));
    } else {
      // Invalid Unicode scalar - mark with invalid UTF-8 pattern
      rune[0] = 0xFF;
    }
  }

  /// @brief Factory function for safe construction
  /// @param scalar The Unicode code point to encode
  /// @return CodePoint if valid, std::nullopt if invalid
  [[nodiscard]] static constexpr std::optional<CodePoint> from_scalar(uint32_t scalar) noexcept {
    CodePoint cp{scalar};
    return cp.is_valid() ? std::optional{cp} : std::nullopt;
  }

  /// @brief Get the number of UTF-8 code units (bytes)
  /// @return Number of valid bytes (0-4), 0 indicates invalid
  [[nodiscard]] constexpr std::size_t count() const noexcept {
    // Check for explicit invalid marker
    if (rune[0] == 0xFF) return 0;

    // Check lead byte pattern to determine length
    if ((rune[0] & 0x80) == 0x00) return 1;  // 0xxxxxxx (1 byte)
    if ((rune[0] & 0xE0) == 0xC0) return 2;  // 110xxxxx (2 bytes)
    if ((rune[0] & 0xF0) == 0xE0) return 3;  // 1110xxxx (3 bytes)
    if ((rune[0] & 0xF8) == 0xF0) return 4;  // 11110xxx (4 bytes)
    return 0;                                // Invalid lead byte
  }

  /// @brief Get a span view of the valid UTF-8 bytes
  /// @return Span covering only the valid bytes (length 1-4)
  [[nodiscard]] constexpr std::span<const uint8_t> units() const noexcept {
    return std::span{rune.data(), count()};
  }

  /// @brief Get direct pointer to the UTF-8 data
  /// @return Pointer to the first byte of the encoded sequence
  /// @note For performance-critical code. Use count() to determine valid length.
  [[nodiscard]] constexpr const uint8_t* data() const noexcept { return rune.data(); }

  /// @brief Decode to Unicode scalar value
  /// @return The Unicode scalar value if valid, std::nullopt if invalid
  [[nodiscard]] constexpr std::optional<uint32_t> to_scalar() const noexcept {
    using namespace limits;

    std::size_t len = count();
    if (len == 0) return std::nullopt;

    uint32_t result;

    if (len == 1) {
      result = rune[0];
    } else if (len == 2) {
      result = ((rune[0] & 0x1F) << 6) | (rune[1] & 0x3F);
    } else if (len == 3) {
      result = ((rune[0] & 0x0F) << 12) | ((rune[1] & 0x3F) << 6) | (rune[2] & 0x3F);
    } else if (len == 4) {
      result = ((rune[0] & 0x07) << 18) | ((rune[1] & 0x3F) << 12) | ((rune[2] & 0x3F) << 6) |
               (rune[3] & 0x3F);
    } else {
      return std::nullopt;
    }

    return result;
  }

  /// @brief Decode to Unicode scalar value without validation
  /// @return The Unicode scalar value, or invalid_scalar if invalid
  /// @warning Precondition: is_valid() must be true. Undefined behavior otherwise.
  /// @note For performance-critical code when validity is already guaranteed.
  [[nodiscard]] constexpr uint32_t to_scalar_unchecked() const noexcept {
    auto result = to_scalar();
    return result.value_or(limits::invalid_scalar);
  }

  /// @brief Check if this represents a valid UTF-8 encoded code point
  /// @return true if valid, false otherwise
  /// @details Validates:
  ///   - Length is in valid range (1-4)
  ///   - Decoded scalar is in valid Unicode range
  ///   - Not a surrogate value
  ///   - No overlong encodings
  [[nodiscard]] constexpr bool is_valid() const noexcept {
    using namespace limits;

    std::size_t len = count();
    if (len == 0 || len > 4) return false;

    auto scalar_opt = to_scalar();
    if (!scalar_opt) return false;

    uint32_t scalar = *scalar_opt;
    if (scalar > max_code_point) return false;
    if (scalar >= surrogate_min && scalar <= surrogate_max) return false;

    // Check for overlong encodings (security issue)
    if (len == 2 && scalar <= utf8_1byte_max) return false;
    if (len == 3 && scalar <= utf8_2byte_max) return false;
    if (len == 4 && scalar <= utf8_3byte_max) return false;

    return true;
  }

  /// @brief Get the size in bytes
  /// @return Size in bytes (same as count() for UTF-8)
  [[nodiscard]] constexpr std::size_t size() const noexcept { return count(); }

  /// @brief Compare with a Unicode scalar value
  /// @param scalar The scalar value to compare with
  /// @return true if this code point represents the given scalar
  constexpr bool operator==(uint32_t scalar) const noexcept {
    return to_scalar_unchecked() == scalar;
  }

  /// @brief Three-way comparison operator
  constexpr auto operator<=>(const CodePoint&) const noexcept = default;

  /// @brief Swap two code points
  friend constexpr void swap(CodePoint& a, CodePoint& b) noexcept { std::swap(a.rune, b.rune); }
};

// ============================================================================
// UTF-16 CodePoint Specialization
// ============================================================================

/// @brief UTF-16 code point representation
/// @tparam E Endianness (must be BE or LE, not None)
/// @details Stores a single Unicode code point encoded as UTF-16 (1-2 units).
/// Total size: exactly 4 bytes (optimal alignment).
/// Handles both BMP characters (single unit) and supplementary characters (surrogate pairs).
/// Length is computed on-demand from surrogate detection.
///
/// @note Construction may create invalid code points. Always check is_valid()
/// after construction, or use from_scalar() factory function for safe construction.
/// Default construction creates U+0000 (null character), which is a valid code point.
template <Endian E>
  requires(MultiByteOriented<Utf16> && E != Endian::None)
struct CodePoint<Utf16, E> {
  using encoding_type = Utf16;
  static constexpr Endian endianness = E;

  std::array<uint16_t, 2> rune{};  ///< UTF-16 encoded units (stored in target endianness)

  /// @brief Default constructor creates a null character (U+0000)
  /// @note U+0000 is a valid Unicode code point.
  constexpr CodePoint() noexcept = default;

  /// @brief Construct from a Unicode scalar value
  /// @param unicode_scalar The Unicode code point to encode (U+0000 to U+10FFFF)
  /// @note May create invalid CodePoint if scalar is out of range or a surrogate.
  ///       Always check is_valid() after construction.
  constexpr explicit CodePoint(uint32_t unicode_scalar) noexcept {
    using namespace limits;

    if (unicode_scalar <= bmp_max) {
      // Single unit (BMP)
      if (unicode_scalar >= surrogate_min && unicode_scalar <= surrogate_max) {
        // Mark as invalid with unambiguous pattern:
        // High surrogate + high surrogate (never valid together)
        rune[0] = to_target_endian(high_surrogate_min);
        rune[1] = to_target_endian(high_surrogate_min);
        return;
      }
      rune[0] = to_target_endian(static_cast<uint16_t>(unicode_scalar));
      rune[1] = 0;  // Second unit unused
    } else if (unicode_scalar <= max_code_point) {
      // Surrogate pair
      unicode_scalar -= surrogate_offset;
      uint16_t high = static_cast<uint16_t>(high_surrogate_min + (unicode_scalar >> 10));
      uint16_t low = static_cast<uint16_t>(low_surrogate_min + (unicode_scalar & 0x3FF));
      rune[0] = to_target_endian(high);
      rune[1] = to_target_endian(low);
    } else {
      // Invalid - mark with invalid pattern (high surrogate + high surrogate)
      rune[0] = to_target_endian(high_surrogate_min);
      rune[1] = to_target_endian(high_surrogate_min);
    }
  }

  /// @brief Factory function for safe construction
  /// @param scalar The Unicode code point to encode
  /// @return CodePoint if valid, std::nullopt if invalid
  [[nodiscard]] static constexpr std::optional<CodePoint> from_scalar(uint32_t scalar) noexcept {
    CodePoint cp{scalar};
    return cp.is_valid() ? std::optional{cp} : std::nullopt;
  }

  /// @brief Get the number of UTF-16 code units
  /// @return Number of valid units (0-2), 0 indicates invalid
  [[nodiscard]] constexpr std::size_t count() const noexcept {
    uint16_t first = from_target_endian(rune[0]);

    if (first == 0) return 1;  // Null character

    // Check if high surrogate (start of pair)
    if (is_high_surrogate(first)) {
      uint16_t second = from_target_endian(rune[1]);
      if (is_low_surrogate(second)) {
        return 2;  // Valid surrogate pair
      }
      return 0;  // Orphaned high surrogate - invalid
    }

    // Check if low surrogate (orphaned - invalid)
    if (is_low_surrogate(first)) {
      return 0;
    }

    // Regular BMP character
    return 1;
  }

  /// @brief Get a span view of the valid UTF-16 units
  /// @return Span covering only the valid units (length 1-2)
  [[nodiscard]] constexpr std::span<const uint16_t> units() const noexcept {
    return std::span{rune.data(), count()};
  }

  /// @brief Get direct pointer to the UTF-16 data
  /// @return Pointer to the first unit of the encoded sequence
  /// @note For performance-critical code. Use count() to determine valid length.
  [[nodiscard]] constexpr const uint16_t* data() const noexcept { return rune.data(); }

  /// @brief Decode to Unicode scalar value
  /// @return The Unicode scalar value if valid, std::nullopt if invalid
  [[nodiscard]] constexpr std::optional<uint32_t> to_scalar() const noexcept {
    using namespace limits;

    std::size_t len = count();
    if (len == 0) return std::nullopt;

    uint16_t first = from_target_endian(rune[0]);

    if (len == 1) {
      return first;
    } else if (len == 2) {
      uint16_t second = from_target_endian(rune[1]);
      uint32_t high = (first - high_surrogate_min) << 10;
      uint32_t low = second - low_surrogate_min;
      return high + low + surrogate_offset;
    }

    return std::nullopt;
  }

  /// @brief Decode to Unicode scalar value without validation
  /// @return The Unicode scalar value, or invalid_scalar if invalid
  /// @warning Precondition: is_valid() must be true. Undefined behavior otherwise.
  /// @note For performance-critical code when validity is already guaranteed.
  [[nodiscard]] constexpr uint32_t to_scalar_unchecked() const noexcept {
    auto result = to_scalar();
    return result.value_or(limits::invalid_scalar);
  }

  /// @brief Check if this represents a valid UTF-16 encoded code point
  /// @return true if valid, false otherwise
  /// @details Validates:
  ///   - Length is in valid range (1-2)
  ///   - Single units are not surrogates
  ///   - Surrogate pairs have valid high and low surrogates
  [[nodiscard]] constexpr bool is_valid() const noexcept {
    using namespace limits;

    std::size_t len = count();
    if (len == 0) return false;

    uint16_t first = from_target_endian(rune[0]);

    if (len == 1) {
      // Single unit - must not be a surrogate
      return !is_surrogate(first);
    } else {  // len == 2
      uint16_t second = from_target_endian(rune[1]);
      // First must be high surrogate, second must be low surrogate
      return is_high_surrogate(first) && is_low_surrogate(second);
    }
  }

  /// @brief Get the size in bytes
  /// @return Size in bytes (count * 2)
  [[nodiscard]] constexpr std::size_t size() const noexcept { return count() * sizeof(uint16_t); }

  /// @brief Compare with a Unicode scalar value
  /// @param scalar The scalar value to compare with
  /// @return true if this code point represents the given scalar
  constexpr bool operator==(uint32_t scalar) const noexcept {
    return to_scalar_unchecked() == scalar;
  }

  /// @brief Three-way comparison operator
  constexpr auto operator<=>(const CodePoint&) const noexcept = default;

  /// @brief Swap two code points
  friend constexpr void swap(CodePoint& a, CodePoint& b) noexcept { std::swap(a.rune, b.rune); }

 private:
  /// @brief Check if a UTF-16 unit is any surrogate (high or low)
  [[nodiscard]] static constexpr bool is_surrogate(uint16_t u) noexcept {
    return u >= 0xD800 && u <= 0xDFFF;
  }

  /// @brief Check if a UTF-16 unit is a high surrogate
  [[nodiscard]] static constexpr bool is_high_surrogate(uint16_t u) noexcept {
    return u >= 0xD800 && u <= 0xDBFF;
  }

  /// @brief Check if a UTF-16 unit is a low surrogate
  [[nodiscard]] static constexpr bool is_low_surrogate(uint16_t u) noexcept {
    return u >= 0xDC00 && u <= 0xDFFF;
  }

  /// @brief Convert value to target endianness
  [[nodiscard]] static constexpr uint16_t to_target_endian(uint16_t v) noexcept {
    if constexpr ((E == Endian::LE && std::endian::native == std::endian::big) ||
                  (E == Endian::BE && std::endian::native == std::endian::little)) {
      return std::byteswap(v);
    } else {
      return v;
    }
  }

  /// @brief Convert value from target endianness to native
  [[nodiscard]] static constexpr uint16_t from_target_endian(uint16_t v) noexcept {
    return to_target_endian(v);  // Swap is symmetric
  }
};

// ============================================================================
// UTF-32 CodePoint Specialization
// ============================================================================

/// @brief UTF-32 code point representation
/// @tparam E Endianness (must be BE or LE, not None)
/// @details Stores a single Unicode code point as a single UTF-32 unit.
/// Total size: exactly 4 bytes (optimal alignment).
/// This is the simplest encoding where one unit always equals one code point.
///
/// @note Construction may create invalid code points. Always check is_valid()
/// after construction, or use from_scalar() factory function for safe construction.
/// Default construction creates U+0000 (null character), which is a valid code point.
template <Endian E>
  requires(MultiByteOriented<Utf32> && E != Endian::None)
struct CodePoint<Utf32, E> {
  using encoding_type = Utf32;
  static constexpr Endian endianness = E;

  uint32_t rune{};  ///< The UTF-32 encoded unit (stored in target endianness)

  /// @brief Default constructor creates a null character (U+0000)
  /// @note U+0000 is a valid Unicode code point.
  constexpr CodePoint() noexcept = default;

  /// @brief Construct from a Unicode scalar value
  /// @param unicode_scalar The Unicode code point to encode (U+0000 to U+10FFFF)
  /// @note May create invalid CodePoint if scalar is out of range or a surrogate.
  ///       Always check is_valid() after construction.
  constexpr explicit CodePoint(uint32_t unicode_scalar) noexcept
      : rune(to_target_endian(unicode_scalar)) {}

  /// @brief Factory function for safe construction
  /// @param scalar The Unicode code point to encode
  /// @return CodePoint if valid, std::nullopt if invalid
  [[nodiscard]] static constexpr std::optional<CodePoint> from_scalar(uint32_t scalar) noexcept {
    CodePoint cp{scalar};
    return cp.is_valid() ? std::optional{cp} : std::nullopt;
  }

  /// @brief Get a span view of the single UTF-32 unit
  /// @return Span covering the single unit
  [[nodiscard]] constexpr std::span<const uint32_t> units() const noexcept {
    return std::span{&rune, 1};
  }

  /// @brief Get direct pointer to the UTF-32 data
  /// @return Pointer to the encoded unit
  [[nodiscard]] constexpr const uint32_t* data() const noexcept { return &rune; }

  /// @brief Decode to Unicode scalar value
  /// @return The Unicode scalar value if valid, std::nullopt if invalid
  [[nodiscard]] constexpr std::optional<uint32_t> to_scalar() const noexcept {
    uint32_t scalar = from_target_endian(rune);
    return is_valid() ? std::optional{scalar} : std::nullopt;
  }

  /// @brief Decode to Unicode scalar value without validation
  /// @return The Unicode scalar value
  /// @warning Precondition: is_valid() must be true. Undefined behavior otherwise.
  /// @note For performance-critical code when validity is already guaranteed.
  [[nodiscard]] constexpr uint32_t to_scalar_unchecked() const noexcept {
    return from_target_endian(rune);
  }

  /// @brief Check if this represents a valid Unicode code point
  /// @return true if valid, false otherwise
  /// @details Validates:
  ///   - Value is in valid Unicode range (0 to 0x10FFFF)
  ///   - Value is not a surrogate (0xD800-0xDFFF)
  [[nodiscard]] constexpr bool is_valid() const noexcept {
    using namespace limits;
    uint32_t scalar = from_target_endian(rune);
    return scalar <= max_code_point && !(scalar >= surrogate_min && scalar <= surrogate_max);
  }

  /// @brief Get the number of UTF-32 code units (always 1)
  /// @return 1
  [[nodiscard]] constexpr std::size_t count() const noexcept { return 1; }

  /// @brief Get the size in bytes (always 4)
  /// @return 4
  [[nodiscard]] constexpr std::size_t size() const noexcept { return sizeof(uint32_t); }

  /// @brief Compare with a Unicode scalar value
  /// @param scalar The scalar value to compare with
  /// @return true if this code point represents the given scalar
  constexpr bool operator==(uint32_t scalar) const noexcept {
    return to_scalar_unchecked() == scalar;
  }

  /// @brief Three-way comparison (compare native values)
  constexpr bool operator==(const CodePoint& other) const noexcept {
    return to_scalar_unchecked() == other.to_scalar_unchecked();
  }

  /// @brief Three-way comparison operator
  constexpr auto operator<=>(const CodePoint& other) const noexcept {
    return to_scalar_unchecked() <=> other.to_scalar_unchecked();
  }

  /// @brief Swap two code points
  friend constexpr void swap(CodePoint& a, CodePoint& b) noexcept { std::swap(a.rune, b.rune); }

 private:
  /// @brief Convert value to target endianness
  [[nodiscard]] static constexpr uint32_t to_target_endian(uint32_t v) noexcept {
    if constexpr ((E == Endian::LE && std::endian::native == std::endian::big) ||
                  (E == Endian::BE && std::endian::native == std::endian::little)) {
      return std::byteswap(v);
    } else {
      return v;
    }
  }

  /// @brief Convert value from target endianness to native
  [[nodiscard]] static constexpr uint32_t from_target_endian(uint32_t v) noexcept {
    return to_target_endian(v);  // Swap is symmetric
  }
};

// ============================================================================
// Type Aliases
// ============================================================================

/// UTF-8 code point (endianness not applicable)
using Utf8CodePoint = CodePoint<Utf8, Endian::None>;

/// UTF-16 code point in big-endian byte order
using Utf16BECodePoint = CodePoint<Utf16, Endian::BE>;

/// UTF-16 code point in little-endian byte order
using Utf16LECodePoint = CodePoint<Utf16, Endian::LE>;

/// UTF-32 code point in big-endian byte order
using Utf32BECodePoint = CodePoint<Utf32, Endian::BE>;

/// UTF-32 code point in little-endian byte order
using Utf32LECodePoint = CodePoint<Utf32, Endian::LE>;

// ============================================================================
// Conversion Functions
// ============================================================================

/// @brief Convert between different UTF encodings and endiannesses
/// @tparam DestCodePoint The destination CodePoint type
/// @tparam SrcCodePoint The source CodePoint type (deduced)
/// @param from The source code point to convert
/// @return The converted code point, or std::nullopt if source is invalid
///
/// @details This function safely converts between any valid CodePoint types.
/// If the source code point is invalid, std::nullopt is returned.
///
/// Example:
/// @code
/// utf::Utf8CodePoint u8{0x1F4A9};
/// auto u16 = utf::convert<utf::Utf16BECodePoint>(u8);
/// if (u16) {
///     // Use *u16
/// }
/// @endcode
template <typename DestCodePoint, typename SrcCodePoint>
  requires IsCodePoint<DestCodePoint> && IsCodePoint<SrcCodePoint>
[[nodiscard]] constexpr std::optional<DestCodePoint> convert(const SrcCodePoint& from) noexcept {
  auto scalar = from.to_scalar();
  if (!scalar) return std::nullopt;
  return DestCodePoint::from_scalar(*scalar);
}

/// @brief Convert between UTF encodings without validation (fast path)
/// @tparam DestCodePoint The destination CodePoint type
/// @tparam SrcCodePoint The source CodePoint type (deduced)
/// @param from The source code point to convert
/// @return The converted code point
///
/// @warning Precondition: from.is_valid() must be true. Undefined behavior otherwise.
/// @note Use this for performance-critical code when validity is guaranteed.
///
/// Example:
/// @code
/// utf::Utf8CodePoint u8{0x41};  // 'A' - known valid
/// auto u32 = utf::convert_unchecked<utf::Utf32BECodePoint>(u8);
/// @endcode
template <typename DestCodePoint, typename SrcCodePoint>
  requires IsCodePoint<DestCodePoint> && IsCodePoint<SrcCodePoint>
[[nodiscard]] constexpr DestCodePoint convert_unchecked(const SrcCodePoint& from) noexcept {
  uint32_t scalar = from.to_scalar_unchecked();
  return DestCodePoint{scalar};
}

/// @brief Convert any CodePoint to UTF-8
/// @param from The source code point
/// @return UTF-8 encoded code point, or std::nullopt if source is invalid
template <typename SrcCodePoint>
  requires IsCodePoint<SrcCodePoint>
[[nodiscard]] constexpr std::optional<Utf8CodePoint> to_utf8(const SrcCodePoint& from) noexcept {
  return convert<Utf8CodePoint>(from);
}

/// @brief Convert any CodePoint to UTF-16 Big Endian
/// @param from The source code point
/// @return UTF-16 BE encoded code point, or std::nullopt if source is invalid
template <typename SrcCodePoint>
  requires IsCodePoint<SrcCodePoint>
[[nodiscard]] constexpr std::optional<Utf16BECodePoint> to_utf16_be(
    const SrcCodePoint& from) noexcept {
  return convert<Utf16BECodePoint>(from);
}

/// @brief Convert any CodePoint to UTF-16 Little Endian
/// @param from The source code point
/// @return UTF-16 LE encoded code point, or std::nullopt if source is invalid
template <typename SrcCodePoint>
  requires IsCodePoint<SrcCodePoint>
[[nodiscard]] constexpr std::optional<Utf16LECodePoint> to_utf16_le(
    const SrcCodePoint& from) noexcept {
  return convert<Utf16LECodePoint>(from);
}

/// @brief Convert any CodePoint to UTF-32 Big Endian
/// @param from The source code point
/// @return UTF-32 BE encoded code point, or std::nullopt if source is invalid
template <typename SrcCodePoint>
  requires IsCodePoint<SrcCodePoint>
[[nodiscard]] constexpr std::optional<Utf32BECodePoint> to_utf32_be(
    const SrcCodePoint& from) noexcept {
  return convert<Utf32BECodePoint>(from);
}

/// @brief Convert any CodePoint to UTF-32 Little Endian
/// @param from The source code point
/// @return UTF-32 LE encoded code point, or std::nullopt if source is invalid
template <typename SrcCodePoint>
  requires IsCodePoint<SrcCodePoint>
[[nodiscard]] constexpr std::optional<Utf32LECodePoint> to_utf32_le(
    const SrcCodePoint& from) noexcept {
  return convert<Utf32LECodePoint>(from);
}

}  // namespace utf

#endif  // UTF_CODEPOINT_HPP