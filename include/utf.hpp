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

#pragma once

/**
 * @file utf.hpp
 * @brief Central UTF Strings library header - main API entry point
 *
 * This is the primary header for the UTF Strings library. It provides a unified
 * namespace and includes all necessary components for working with UTF-8, UTF-16,
 * and UTF-32 strings with explicit endianness control.
 *
 * Features:
 * - UTF-8/16/32 string views and containers
 * - Small String Optimization (SSO) - strings <= 32 bytes stored on stack
 * - Iterator support for code point traversal
 * - String conversion between encodings
 * - String concatenation operators
 * - Validation and sanitization
 * - constexpr and noexcept throughout for zero runtime overhead
 * - Range-based operations
 * - Factory methods for creating strings from byte arrays
 *
 * Requirements:
 * - C++23 or later
 * - Standard library support for: <string>, <string_view>, <ranges>
 *
 * Example Usage:
 * @code
 *   #include <utf.hpp>
 *
 *   // Create UTF-8 string view
 *   utf::string::Utf8StringView sv{u8"Hello 世界 💩"};
 *
 *   // Iterate over code points
 *   for (auto cp : sv) {
 *       if (cp.is_valid()) {
 *           auto scalar = cp.to_scalar();
 *           // ...
 *       }
 *   }
 *
 *   // Convert between encodings
 *   utf::string::Utf32BEString u32{0x1F4A9};
 *   utf::string::Utf8String u8{u32};  // Converting constructor
 *   u8 = u32;                         // Converting assignment
 *
 *   // Factory methods from byte arrays
 *   std::array<uint8_t, 4> utf8_bytes = {0x48, 0x65, 0x6C, 0x6C};  // "Hell"
 *   auto utf8_str = utf::string::Utf8String::from_bytes(utf8_bytes.data(), utf8_bytes.size());
 *   if (utf8_str.has_value()) {
 *       // Use the string
 *   }
 *
 *   // String concatenation
 *   auto combined = u8 + u8;
 *   u8 += utf::Utf8CodePoint{0x21};  // Append '!'
 * @endcode
 *
 * @version 0.0.2
 * @author William Sollers
 * @copyright BSD 2-Clause License
 */

#ifndef UTF_HPP
#define UTF_HPP

// Version information - this is the authoritative version for the library
#define UTF_VERSION_MAJOR 0
#define UTF_VERSION_MINOR 0
#define UTF_VERSION_PATCH 2
#define UTF_VERSION_STRING "0.0.2"
#define UTF_VERSION_NUMBER 2

// Include all UTF library components
#include "utf/export.hpp"          // Platform-specific export macros
#include "utf/utf_codepoints.hpp"  // Core UTF codepoint functionality
#include "utf/utf_streams.hpp"     // UTF streams and BOM support
#include "utf/utf_strings.hpp"     // UTF string types and operations
#include "utf/version.hpp"         // Detailed version information

/**
 * @brief Root namespace for the UTF Strings library
 *
 * This namespace contains all UTF-related functionality including:
 * - Version information (utf::version)
 * - UTF codepoint types and operations
 * - UTF string types and string views
 * - Conversion utilities between encodings
 * - Factory methods for creating strings from raw byte data
 *
 * All library functionality is accessed through this namespace or its
 * nested namespaces (such as utf::string for string types).
 */
namespace utf {

/**
 * @brief Get the library version as a string
 * @return Version string in format "major.minor.patch"
 */
inline constexpr const char* get_version() { return UTF_VERSION_STRING; }

/**
 * @brief Get the library version as an integer
 * @return Version number in format MAJOR*10000 + MINOR*100 + PATCH
 */
inline constexpr int get_version_number() { return UTF_VERSION_NUMBER; }

/**
 * @brief Check if the library version is at least the specified version
 * @param major Major version number
 * @param minor Minor version number
 * @param patch Patch version number
 * @return true if current version >= specified version
 */
inline constexpr bool version_at_least(int major, int minor = 0, int patch = 0) {
  const int required = major * 10000 + minor * 100 + patch;
  return UTF_VERSION_NUMBER >= required;
}

}  // namespace utf

// Convenience macros for version checking
#define UTF_VERSION_AT_LEAST(major, minor, patch) \
  (UTF_VERSION_NUMBER >= ((major) * 10000 + (minor) * 100 + (patch)))

#endif  // UTF_HPP