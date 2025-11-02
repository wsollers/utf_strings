#pragma once

/**
 * @file version.hpp
 * @brief UTF Strings library version information
 */

#ifndef UTF_STRINGS_VERSION_HPP
#define UTF_STRINGS_VERSION_HPP

namespace utf {

/**
 * @brief Version information for the UTF Strings library
 */
struct version {
  static constexpr int major = 0;
  static constexpr int minor = 0;
  static constexpr int patch = 1;

  /**
   * @brief Get version as string in format "major.minor.patch"
   */
  static constexpr const char* string() { return "0.0.1"; }

  /**
   * @brief Get version as integer in format MAJOR*10000 + MINOR*100 + PATCH
   */
  static constexpr int number() { return major * 10000 + minor * 100 + patch; }
};

}  // namespace utf

// Convenience macros
#define UTF_STRINGS_VERSION_MAJOR 0
#define UTF_STRINGS_VERSION_MINOR 0
#define UTF_STRINGS_VERSION_PATCH 1
#define UTF_STRINGS_VERSION_STRING "0.0.1"
#define UTF_STRINGS_VERSION_NUMBER 1

#endif  // UTF_STRINGS_VERSION_HPP