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