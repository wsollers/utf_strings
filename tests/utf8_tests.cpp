
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

#include <gtest/gtest.h>

#include <string>

#include "utf/utf_strings.hpp"

using namespace utf;

TEST(UTF8, ValidAscii) {
  utf8_string s{std::u8string{u8"hello"}};
  EXPECT_TRUE(s.valid());
  auto n = s.length();
  ASSERT_TRUE(n.has_value());
  EXPECT_EQ(*n, 5u);
}

TEST(UTF8, ValidMultibyte) {
  // Use explicit UTF-8 byte sequences to avoid source encoding issues on Windows
  // "Héllø 🌍" = H(0x48) é(0xC3,0xA9) l(0x6C) l(0x6C) ø(0xC3,0xB8) space(0x20)
  // 🌍(0xF0,0x9F,0x8C,0x8D)
  std::u8string utf8_bytes;
  utf8_bytes.push_back(0x48);  // H
  utf8_bytes.push_back(0xC3);
  utf8_bytes.push_back(0xA9);  // é (U+00E9)
  utf8_bytes.push_back(0x6C);  // l
  utf8_bytes.push_back(0x6C);  // l
  utf8_bytes.push_back(0xC3);
  utf8_bytes.push_back(0xB8);  // ø (U+00F8)
  utf8_bytes.push_back(0x20);  // space
  utf8_bytes.push_back(0xF0);
  utf8_bytes.push_back(0x9F);  // 🌍 (U+1F30D)
  utf8_bytes.push_back(0x8C);
  utf8_bytes.push_back(0x8D);

  utf8_string s{utf8_bytes};
  ASSERT_TRUE(s.valid());
  auto n = s.length();
  ASSERT_TRUE(n.has_value());
  EXPECT_EQ(*n, 7u);  // H-é-l-l-ø-space-🌍 = 7 code points
  auto u32 = s.to_u32();
  ASSERT_TRUE(u32.has_value());
  EXPECT_EQ((*u32)[0], U'H');
  EXPECT_EQ((*u32)[1], U'\u00E9');      // é (U+00E9)
  EXPECT_EQ((*u32)[4], U'\u00F8');      // ø (U+00F8)
  EXPECT_EQ((*u32)[6], U'\U0001F30D');  // 🌍 (U+1F30D)
}

TEST(UTF8, RejectOverlong) {
  std::u8string overlong;
  overlong.push_back(static_cast<char8_t>(0xC0));
  overlong.push_back(static_cast<char8_t>(0xAF));
  EXPECT_FALSE((valid<char8_t, endian::big>(overlong)));
}

TEST(UTF16BE, SurrogatePair) {
  std::u16string native{0xD83C, 0xDF0D};  // 🌍
  auto be = utf16be_string::from_native(native);
  EXPECT_TRUE(be.valid());
  auto n = be.length();
  ASSERT_TRUE(n.has_value());
  EXPECT_EQ(*n, 1u);
  auto u32 = be.to_u32();
  ASSERT_TRUE(u32.has_value());
  EXPECT_EQ((*u32)[0], U'\U0001F30D');  // 🌍 (U+1F30D)
  auto round = be.to_native();
  EXPECT_EQ(round, native);
}

TEST(UTF16LE, SurrogatePair) {
  std::u16string native{0xD83C, 0xDF0D};
  auto le = utf16le_string::from_native(native);
  EXPECT_TRUE(le.valid());
  EXPECT_EQ(*le.length(), 1u);
}

TEST(UTF32BE, Basic) {
  std::u32string nat{U'H', U'\u00E9', U'\u00F8', U'\U0001F30D'};  // H, é, ø, 🌍
  auto be = utf32be_string::from_native(nat);
  EXPECT_TRUE(be.valid());
  EXPECT_EQ(*be.length(), 4u);
  auto round = be.to_native();
  EXPECT_EQ(round, nat);
}
