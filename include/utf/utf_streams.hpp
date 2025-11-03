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

/**
 * @file utf_streams.hpp
 * @brief UTF streams and Byte Order Mark (BOM) support
 */

#ifndef UTF_STREAMS_HPP
#define UTF_STREAMS_HPP

#include <cstdint>

namespace utf {

/**
 * @brief Byte Order Mark (BOM) constants for Unicode encodings
 */
namespace bom {

/**
 * @brief UTF-8 Byte Order Mark
 * Sequence: EF BB BF
 */
constexpr uint8_t utf8[] = {0xEF, 0xBB, 0xBF};

/**
 * @brief UTF-16 Big Endian Byte Order Mark
 * Sequence: FE FF
 */
constexpr uint8_t utf16_be[] = {0xFE, 0xFF};

/**
 * @brief UTF-16 Little Endian Byte Order Mark
 * Sequence: FF FE
 */
constexpr uint8_t utf16_le[] = {0xFF, 0xFE};

/**
 * @brief UTF-32 Big Endian Byte Order Mark
 * Sequence: 00 00 FE FF
 */
constexpr uint8_t utf32_be[] = {0x00, 0x00, 0xFE, 0xFF};

/**
 * @brief UTF-32 Little Endian Byte Order Mark
 * Sequence: FF FE 00 00
 */
constexpr uint8_t utf32_le[] = {0xFF, 0xFE, 0x00, 0x00};

/**
 * @brief Size constants for BOM sequences
 */
namespace size {
constexpr std::size_t utf8 = 3;
constexpr std::size_t utf16 = 2;
constexpr std::size_t utf32 = 4;
}  // namespace size

}  // namespace bom

/**
 * @brief UTF streams functionality
 *
 * This namespace will contain stream-based UTF processing functionality
 * including input/output streams, formatters, and conversion utilities.
 */
namespace streams {
// Future stream implementations will go here
}  // namespace streams

}  // namespace utf

#endif  // UTF_STREAMS_HPP