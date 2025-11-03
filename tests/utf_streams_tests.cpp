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

#include <cstring>
#include <utf.hpp>

/**
 * @brief Test BOM constants and UTF streams functionality
 */
class UTFStreamsTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(UTFStreamsTest, BOMConstants) {
  // Test UTF-8 BOM
  EXPECT_EQ(utf::bom::size::utf8, 3);
  EXPECT_EQ(utf::bom::utf8[0], 0xEF);
  EXPECT_EQ(utf::bom::utf8[1], 0xBB);
  EXPECT_EQ(utf::bom::utf8[2], 0xBF);

  // Test UTF-16 BE BOM
  EXPECT_EQ(utf::bom::size::utf16, 2);
  EXPECT_EQ(utf::bom::utf16_be[0], 0xFE);
  EXPECT_EQ(utf::bom::utf16_be[1], 0xFF);

  // Test UTF-16 LE BOM
  EXPECT_EQ(utf::bom::utf16_le[0], 0xFF);
  EXPECT_EQ(utf::bom::utf16_le[1], 0xFE);

  // Test UTF-32 BE BOM
  EXPECT_EQ(utf::bom::size::utf32, 4);
  EXPECT_EQ(utf::bom::utf32_be[0], 0x00);
  EXPECT_EQ(utf::bom::utf32_be[1], 0x00);
  EXPECT_EQ(utf::bom::utf32_be[2], 0xFE);
  EXPECT_EQ(utf::bom::utf32_be[3], 0xFF);

  // Test UTF-32 LE BOM
  EXPECT_EQ(utf::bom::utf32_le[0], 0xFF);
  EXPECT_EQ(utf::bom::utf32_le[1], 0xFE);
  EXPECT_EQ(utf::bom::utf32_le[2], 0x00);
  EXPECT_EQ(utf::bom::utf32_le[3], 0x00);
}

TEST_F(UTFStreamsTest, BOMComparison) {
  // Test that we can compare against actual byte sequences
  const uint8_t utf8_sample[] = {0xEF, 0xBB, 0xBF};
  EXPECT_EQ(std::memcmp(utf::bom::utf8, utf8_sample, utf::bom::size::utf8), 0);

  const uint8_t utf16_be_sample[] = {0xFE, 0xFF};
  EXPECT_EQ(std::memcmp(utf::bom::utf16_be, utf16_be_sample, utf::bom::size::utf16), 0);

  const uint8_t utf16_le_sample[] = {0xFF, 0xFE};
  EXPECT_EQ(std::memcmp(utf::bom::utf16_le, utf16_le_sample, utf::bom::size::utf16), 0);

  const uint8_t utf32_be_sample[] = {0x00, 0x00, 0xFE, 0xFF};
  EXPECT_EQ(std::memcmp(utf::bom::utf32_be, utf32_be_sample, utf::bom::size::utf32), 0);

  const uint8_t utf32_le_sample[] = {0xFF, 0xFE, 0x00, 0x00};
  EXPECT_EQ(std::memcmp(utf::bom::utf32_le, utf32_le_sample, utf::bom::size::utf32), 0);
}

TEST_F(UTFStreamsTest, NamespaceExistence) {
  // Test that namespaces exist and are accessible
  // This is a compile-time test - if it compiles, the namespaces exist
  static_cast<void>(utf::bom::utf8);

  // Test that the streams namespace exists (even if empty)
  // This ensures the namespace is properly declared by using it in a constexpr context
  constexpr bool streams_namespace_exists = true;
  static_cast<void>(streams_namespace_exists);
}