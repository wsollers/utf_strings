
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

#include <utf.hpp>

using namespace utf;

TEST(UTF8, ValidAscii) {
  // Test individual ASCII characters with new CodePoint API
  auto cp_h = Utf8CodePoint::from_scalar('h');
  auto cp_e = Utf8CodePoint::from_scalar('e');
  auto cp_l1 = Utf8CodePoint::from_scalar('l');
  auto cp_l2 = Utf8CodePoint::from_scalar('l');
  auto cp_o = Utf8CodePoint::from_scalar('o');

  ASSERT_TRUE(cp_h.has_value());
  ASSERT_TRUE(cp_e.has_value());
  ASSERT_TRUE(cp_l1.has_value());
  ASSERT_TRUE(cp_l2.has_value());
  ASSERT_TRUE(cp_o.has_value());

  EXPECT_TRUE(cp_h->is_valid());
  EXPECT_TRUE(cp_e->is_valid());
  EXPECT_TRUE(cp_l1->is_valid());
  EXPECT_TRUE(cp_l2->is_valid());
  EXPECT_TRUE(cp_o->is_valid());

  // Each ASCII character should be 1 byte
  EXPECT_EQ(cp_h->count(), 1u);
  EXPECT_EQ(cp_e->count(), 1u);
  EXPECT_EQ(cp_l1->count(), 1u);
  EXPECT_EQ(cp_l2->count(), 1u);
  EXPECT_EQ(cp_o->count(), 1u);
}

TEST(UTF8, ValidMultibyte) {
  // Test individual multibyte characters with new CodePoint API
  // H (0x48)
  auto cp_H = Utf8CodePoint::from_scalar(0x48);
  ASSERT_TRUE(cp_H.has_value());
  EXPECT_TRUE(cp_H->is_valid());
  EXPECT_EQ(cp_H->count(), 1u);

  // é (U+00E9) - 2 bytes in UTF-8
  auto cp_e = Utf8CodePoint::from_scalar(0x00E9);
  ASSERT_TRUE(cp_e.has_value());
  EXPECT_TRUE(cp_e->is_valid());
  EXPECT_EQ(cp_e->count(), 2u);
  EXPECT_EQ(cp_e->to_scalar_unchecked(), 0x00E9);

  // ø (U+00F8) - 2 bytes in UTF-8
  auto cp_o = Utf8CodePoint::from_scalar(0x00F8);
  ASSERT_TRUE(cp_o.has_value());
  EXPECT_TRUE(cp_o->is_valid());
  EXPECT_EQ(cp_o->count(), 2u);
  EXPECT_EQ(cp_o->to_scalar_unchecked(), 0x00F8);

  // 🌍 (U+1F30D) - 4 bytes in UTF-8
  auto cp_world = Utf8CodePoint::from_scalar(0x1F30D);
  ASSERT_TRUE(cp_world.has_value());
  EXPECT_TRUE(cp_world->is_valid());
  EXPECT_EQ(cp_world->count(), 4u);
  EXPECT_EQ(cp_world->to_scalar_unchecked(), 0x1F30D);
}

TEST(UTF8, InvalidSurrogate) {
  // Test that surrogates are properly rejected
  auto cp = Utf8CodePoint::from_scalar(0xD800);
  EXPECT_FALSE(cp.has_value());  // Surrogates should be invalid

  auto cp2 = Utf8CodePoint::from_scalar(0xDFFF);
  EXPECT_FALSE(cp2.has_value());  // High end of surrogate range should also be invalid
}

TEST(UTF16BE, BasicBMP) {
  // Test Basic Multilingual Plane characters (no surrogate needed)
  auto cp_H = Utf16BECodePoint::from_scalar(U'H');
  ASSERT_TRUE(cp_H.has_value());
  EXPECT_TRUE(cp_H->is_valid());
  EXPECT_EQ(cp_H->count(), 1u);  // Single unit
  EXPECT_EQ(cp_H->size(), 2u);   // 2 bytes
  EXPECT_EQ(cp_H->to_scalar_unchecked(), U'H');

  // Test accented character é (U+00E9)
  auto cp_e = Utf16BECodePoint::from_scalar(0x00E9);
  ASSERT_TRUE(cp_e.has_value());
  EXPECT_TRUE(cp_e->is_valid());
  EXPECT_EQ(cp_e->count(), 1u);
  EXPECT_EQ(cp_e->to_scalar_unchecked(), 0x00E9);
}

TEST(UTF16BE, SurrogatePair) {
  // 🌍 (U+1F30D) requires surrogate pair in UTF-16
  auto cp = Utf16BECodePoint::from_scalar(0x1F30D);
  ASSERT_TRUE(cp.has_value());
  EXPECT_TRUE(cp->is_valid());
  EXPECT_EQ(cp->count(), 2u);  // Should be 2 units (surrogate pair)
  EXPECT_EQ(cp->size(), 4u);   // 4 bytes total
  EXPECT_EQ(cp->to_scalar_unchecked(), 0x1F30D);
}

TEST(UTF16BE, InvalidSurrogate) {
  // Test that individual surrogates are rejected
  auto high_surrogate = Utf16BECodePoint::from_scalar(0xD800);
  EXPECT_FALSE(high_surrogate.has_value());

  auto low_surrogate = Utf16BECodePoint::from_scalar(0xDC00);
  EXPECT_FALSE(low_surrogate.has_value());

  auto mid_surrogate = Utf16BECodePoint::from_scalar(0xDBFF);
  EXPECT_FALSE(mid_surrogate.has_value());
}

TEST(UTF16LE, BasicBMP) {
  // Test Basic Multilingual Plane characters (no surrogate needed)
  auto cp_A = Utf16LECodePoint::from_scalar(U'A');
  ASSERT_TRUE(cp_A.has_value());
  EXPECT_TRUE(cp_A->is_valid());
  EXPECT_EQ(cp_A->count(), 1u);  // Single unit
  EXPECT_EQ(cp_A->size(), 2u);   // 2 bytes
  EXPECT_EQ(cp_A->to_scalar_unchecked(), U'A');

  // Test Greek letter Ω (U+03A9)
  auto cp_omega = Utf16LECodePoint::from_scalar(0x03A9);
  ASSERT_TRUE(cp_omega.has_value());
  EXPECT_TRUE(cp_omega->is_valid());
  EXPECT_EQ(cp_omega->count(), 1u);
  EXPECT_EQ(cp_omega->to_scalar_unchecked(), 0x03A9);
}

TEST(UTF16LE, SurrogatePair) {
  // Same test as BE but for little endian
  auto cp = Utf16LECodePoint::from_scalar(0x1F30D);
  ASSERT_TRUE(cp.has_value());
  EXPECT_TRUE(cp->is_valid());
  EXPECT_EQ(cp->count(), 2u);
  EXPECT_EQ(cp->size(), 4u);
  EXPECT_EQ(cp->to_scalar_unchecked(), 0x1F30D);
}

TEST(UTF16LE, InvalidSurrogate) {
  // Test that individual surrogates are rejected (same as BE)
  auto high_surrogate = Utf16LECodePoint::from_scalar(0xD83C);
  EXPECT_FALSE(high_surrogate.has_value());

  auto low_surrogate = Utf16LECodePoint::from_scalar(0xDF0D);
  EXPECT_FALSE(low_surrogate.has_value());
}

TEST(UTF32BE, Basic) {
  // Test basic UTF-32 characters
  auto cp_H = Utf32BECodePoint::from_scalar(U'H');
  auto cp_e = Utf32BECodePoint::from_scalar(U'\u00E9');          // é
  auto cp_o = Utf32BECodePoint::from_scalar(U'\u00F8');          // ø
  auto cp_world = Utf32BECodePoint::from_scalar(U'\U0001F30D');  // 🌍

  ASSERT_TRUE(cp_H.has_value());
  ASSERT_TRUE(cp_e.has_value());
  ASSERT_TRUE(cp_o.has_value());
  ASSERT_TRUE(cp_world.has_value());

  // All UTF-32 code points should have count=1 and size=4
  EXPECT_EQ(cp_H->count(), 1u);
  EXPECT_EQ(cp_e->count(), 1u);
  EXPECT_EQ(cp_o->count(), 1u);
  EXPECT_EQ(cp_world->count(), 1u);

  EXPECT_EQ(cp_H->size(), 4u);
  EXPECT_EQ(cp_e->size(), 4u);
  EXPECT_EQ(cp_o->size(), 4u);
  EXPECT_EQ(cp_world->size(), 4u);
}

TEST(UTF32BE, SingleCharacters) {
  // Test various Unicode ranges in UTF-32 BE

  // ASCII
  auto cp_Z = Utf32BECodePoint::from_scalar(U'Z');
  ASSERT_TRUE(cp_Z.has_value());
  EXPECT_TRUE(cp_Z->is_valid());
  EXPECT_EQ(cp_Z->count(), 1u);
  EXPECT_EQ(cp_Z->size(), 4u);
  EXPECT_EQ(cp_Z->to_scalar_unchecked(), U'Z');

  // Latin-1 Supplement
  auto cp_cedilla = Utf32BECodePoint::from_scalar(0x00E7);  // ç
  ASSERT_TRUE(cp_cedilla.has_value());
  EXPECT_TRUE(cp_cedilla->is_valid());
  EXPECT_EQ(cp_cedilla->to_scalar_unchecked(), 0x00E7);

  // CJK
  auto cp_chinese = Utf32BECodePoint::from_scalar(0x4E2D);  // 中
  ASSERT_TRUE(cp_chinese.has_value());
  EXPECT_TRUE(cp_chinese->is_valid());
  EXPECT_EQ(cp_chinese->to_scalar_unchecked(), 0x4E2D);

  // Emoji (outside BMP)
  auto cp_rocket = Utf32BECodePoint::from_scalar(0x1F680);  // 🚀
  ASSERT_TRUE(cp_rocket.has_value());
  EXPECT_TRUE(cp_rocket->is_valid());
  EXPECT_EQ(cp_rocket->to_scalar_unchecked(), 0x1F680);
}

TEST(UTF32BE, InvalidCodePoints) {
  // Test that invalid Unicode scalars are rejected

  // Beyond Unicode range
  auto cp_invalid = Utf32BECodePoint::from_scalar(0x110000);
  EXPECT_FALSE(cp_invalid.has_value());

  // Surrogate range (invalid as scalars)
  auto cp_surrogate1 = Utf32BECodePoint::from_scalar(0xD800);
  EXPECT_FALSE(cp_surrogate1.has_value());

  auto cp_surrogate2 = Utf32BECodePoint::from_scalar(0xDFFF);
  EXPECT_FALSE(cp_surrogate2.has_value());
}

TEST(UTF32LE, SingleCharacters) {
  // Test various Unicode ranges in UTF-32 LE

  // Musical symbol
  auto cp_treble = Utf32LECodePoint::from_scalar(0x1D11E);  // 𝄞
  ASSERT_TRUE(cp_treble.has_value());
  EXPECT_TRUE(cp_treble->is_valid());
  EXPECT_EQ(cp_treble->count(), 1u);
  EXPECT_EQ(cp_treble->size(), 4u);
  EXPECT_EQ(cp_treble->to_scalar_unchecked(), 0x1D11E);

  // Mathematical symbol
  auto cp_integral = Utf32LECodePoint::from_scalar(0x222B);  // ∫
  ASSERT_TRUE(cp_integral.has_value());
  EXPECT_TRUE(cp_integral->is_valid());
  EXPECT_EQ(cp_integral->to_scalar_unchecked(), 0x222B);
}

TEST(UTF32LE, InvalidCodePoints) {
  // Test same invalid cases as BE but for LE
  auto cp_invalid = Utf32LECodePoint::from_scalar(0x110000);
  EXPECT_FALSE(cp_invalid.has_value());

  auto cp_surrogate = Utf32LECodePoint::from_scalar(0xD800);
  EXPECT_FALSE(cp_surrogate.has_value());
}

TEST(Conversion, AllEncodingsASCII) {
  // Test ASCII character conversion across all encodings
  uint32_t ascii_char = U'A';

  // Create in each encoding
  auto utf8 = Utf8CodePoint::from_scalar(ascii_char);
  auto utf16be = Utf16BECodePoint::from_scalar(ascii_char);
  auto utf16le = Utf16LECodePoint::from_scalar(ascii_char);
  auto utf32be = Utf32BECodePoint::from_scalar(ascii_char);
  auto utf32le = Utf32LECodePoint::from_scalar(ascii_char);

  ASSERT_TRUE(utf8.has_value());
  ASSERT_TRUE(utf16be.has_value());
  ASSERT_TRUE(utf16le.has_value());
  ASSERT_TRUE(utf32be.has_value());
  ASSERT_TRUE(utf32le.has_value());

  // Test all conversions from UTF-8
  auto u8_to_u16be = convert<Utf16BECodePoint>(*utf8);
  auto u8_to_u16le = convert<Utf16LECodePoint>(*utf8);
  auto u8_to_u32be = convert<Utf32BECodePoint>(*utf8);
  auto u8_to_u32le = convert<Utf32LECodePoint>(*utf8);

  ASSERT_TRUE(u8_to_u16be.has_value());
  ASSERT_TRUE(u8_to_u16le.has_value());
  ASSERT_TRUE(u8_to_u32be.has_value());
  ASSERT_TRUE(u8_to_u32le.has_value());

  EXPECT_EQ(u8_to_u16be->to_scalar_unchecked(), ascii_char);
  EXPECT_EQ(u8_to_u16le->to_scalar_unchecked(), ascii_char);
  EXPECT_EQ(u8_to_u32be->to_scalar_unchecked(), ascii_char);
  EXPECT_EQ(u8_to_u32le->to_scalar_unchecked(), ascii_char);
}

TEST(Conversion, AllEncodingsMultibyte) {
  // Test multibyte character conversion (é - U+00E9)
  uint32_t multibyte_char = 0x00E9;

  auto utf8 = Utf8CodePoint::from_scalar(multibyte_char);
  ASSERT_TRUE(utf8.has_value());

  // Convert UTF-8 to all other encodings
  auto to_u16be = convert<Utf16BECodePoint>(*utf8);
  auto to_u16le = convert<Utf16LECodePoint>(*utf8);
  auto to_u32be = convert<Utf32BECodePoint>(*utf8);
  auto to_u32le = convert<Utf32LECodePoint>(*utf8);

  ASSERT_TRUE(to_u16be.has_value());
  ASSERT_TRUE(to_u16le.has_value());
  ASSERT_TRUE(to_u32be.has_value());
  ASSERT_TRUE(to_u32le.has_value());

  // Verify all produce the same scalar
  EXPECT_EQ(to_u16be->to_scalar_unchecked(), multibyte_char);
  EXPECT_EQ(to_u16le->to_scalar_unchecked(), multibyte_char);
  EXPECT_EQ(to_u32be->to_scalar_unchecked(), multibyte_char);
  EXPECT_EQ(to_u32le->to_scalar_unchecked(), multibyte_char);

  // Test UTF-16 BE to all others
  auto u16be_to_u8 = convert<Utf8CodePoint>(*to_u16be);
  auto u16be_to_u16le = convert<Utf16LECodePoint>(*to_u16be);
  auto u16be_to_u32be = convert<Utf32BECodePoint>(*to_u16be);
  auto u16be_to_u32le = convert<Utf32LECodePoint>(*to_u16be);

  ASSERT_TRUE(u16be_to_u8.has_value());
  ASSERT_TRUE(u16be_to_u16le.has_value());
  ASSERT_TRUE(u16be_to_u32be.has_value());
  ASSERT_TRUE(u16be_to_u32le.has_value());

  EXPECT_EQ(u16be_to_u8->to_scalar_unchecked(), multibyte_char);
  EXPECT_EQ(u16be_to_u16le->to_scalar_unchecked(), multibyte_char);
  EXPECT_EQ(u16be_to_u32be->to_scalar_unchecked(), multibyte_char);
  EXPECT_EQ(u16be_to_u32le->to_scalar_unchecked(), multibyte_char);
}

TEST(Conversion, AllEncodingsSurrogatePair) {
  // Test emoji that requires surrogate pair in UTF-16 (🌍 - U+1F30D)
  uint32_t emoji_char = 0x1F30D;

  // Start with UTF-8
  auto utf8 = Utf8CodePoint::from_scalar(emoji_char);
  ASSERT_TRUE(utf8.has_value());
  EXPECT_EQ(utf8->count(), 4u);  // 4 bytes in UTF-8

  // Convert to UTF-16 (both endiannesses)
  auto to_u16be = convert<Utf16BECodePoint>(*utf8);
  auto to_u16le = convert<Utf16LECodePoint>(*utf8);

  ASSERT_TRUE(to_u16be.has_value());
  ASSERT_TRUE(to_u16le.has_value());
  EXPECT_EQ(to_u16be->count(), 2u);  // Surrogate pair
  EXPECT_EQ(to_u16le->count(), 2u);  // Surrogate pair
  EXPECT_EQ(to_u16be->to_scalar_unchecked(), emoji_char);
  EXPECT_EQ(to_u16le->to_scalar_unchecked(), emoji_char);

  // Convert to UTF-32 (both endiannesses)
  auto to_u32be = convert<Utf32BECodePoint>(*utf8);
  auto to_u32le = convert<Utf32LECodePoint>(*utf8);

  ASSERT_TRUE(to_u32be.has_value());
  ASSERT_TRUE(to_u32le.has_value());
  EXPECT_EQ(to_u32be->count(), 1u);  // Single unit
  EXPECT_EQ(to_u32le->count(), 1u);  // Single unit
  EXPECT_EQ(to_u32be->to_scalar_unchecked(), emoji_char);
  EXPECT_EQ(to_u32le->to_scalar_unchecked(), emoji_char);
}

TEST(Conversion, RoundTripAllCombinations) {
  // Test round-trip conversions for all encoding combinations
  uint32_t test_scalars[] = {
      0x41,    // ASCII 'A'
      0x00E9,  // Latin é (2 bytes UTF-8, 1 unit UTF-16)
      0x03A9,  // Greek Ω (3 bytes UTF-8, 1 unit UTF-16)
      0x1F30D  // Emoji 🌍 (4 bytes UTF-8, 2 units UTF-16)
  };

  for (uint32_t scalar : test_scalars) {
    // Create original in UTF-8
    auto original = Utf8CodePoint::from_scalar(scalar);
    ASSERT_TRUE(original.has_value()) << "Failed to create UTF-8 for scalar " << std::hex << scalar;

    // Round trip through UTF-16 BE
    auto via_u16be = convert<Utf16BECodePoint>(*original);
    ASSERT_TRUE(via_u16be.has_value());
    auto back_from_u16be = convert<Utf8CodePoint>(*via_u16be);
    ASSERT_TRUE(back_from_u16be.has_value());
    EXPECT_EQ(back_from_u16be->to_scalar_unchecked(), scalar)
        << "UTF-8 -> UTF-16BE -> UTF-8 failed for " << std::hex << scalar;

    // Round trip through UTF-16 LE
    auto via_u16le = convert<Utf16LECodePoint>(*original);
    ASSERT_TRUE(via_u16le.has_value());
    auto back_from_u16le = convert<Utf8CodePoint>(*via_u16le);
    ASSERT_TRUE(back_from_u16le.has_value());
    EXPECT_EQ(back_from_u16le->to_scalar_unchecked(), scalar)
        << "UTF-8 -> UTF-16LE -> UTF-8 failed for " << std::hex << scalar;

    // Round trip through UTF-32 BE
    auto via_u32be = convert<Utf32BECodePoint>(*original);
    ASSERT_TRUE(via_u32be.has_value());
    auto back_from_u32be = convert<Utf8CodePoint>(*via_u32be);
    ASSERT_TRUE(back_from_u32be.has_value());
    EXPECT_EQ(back_from_u32be->to_scalar_unchecked(), scalar)
        << "UTF-8 -> UTF-32BE -> UTF-8 failed for " << std::hex << scalar;

    // Round trip through UTF-32 LE
    auto via_u32le = convert<Utf32LECodePoint>(*original);
    ASSERT_TRUE(via_u32le.has_value());
    auto back_from_u32le = convert<Utf8CodePoint>(*via_u32le);
    ASSERT_TRUE(back_from_u32le.has_value());
    EXPECT_EQ(back_from_u32le->to_scalar_unchecked(), scalar)
        << "UTF-8 -> UTF-32LE -> UTF-8 failed for " << std::hex << scalar;
  }
}

TEST(Conversion, InvalidSourceReturnsNullopt) {
  // Test that converting from an invalid code point returns nullopt
  Utf8CodePoint invalid_utf8{0xD800};  // Invalid surrogate
  EXPECT_FALSE(invalid_utf8.is_valid());

  auto result = convert<Utf16BECodePoint>(invalid_utf8);
  EXPECT_FALSE(result.has_value());

  // Test with UTF-32 as well
  Utf32BECodePoint invalid_utf32{0x110000};  // Beyond Unicode range
  EXPECT_FALSE(invalid_utf32.is_valid());

  auto result2 = convert<Utf8CodePoint>(invalid_utf32);
  EXPECT_FALSE(result2.has_value());
}

TEST(Conversion, ConvenienceFunctions) {
  // Test the convenience conversion functions
  auto utf8 = Utf8CodePoint::from_scalar(0x1F680);  // 🚀
  ASSERT_TRUE(utf8.has_value());

  // Test to_utf16_be
  auto u16be = to_utf16_be(*utf8);
  ASSERT_TRUE(u16be.has_value());
  EXPECT_EQ(u16be->to_scalar_unchecked(), 0x1F680);

  // Test to_utf16_le
  auto u16le = to_utf16_le(*utf8);
  ASSERT_TRUE(u16le.has_value());
  EXPECT_EQ(u16le->to_scalar_unchecked(), 0x1F680);

  // Test to_utf32_be
  auto u32be = to_utf32_be(*utf8);
  ASSERT_TRUE(u32be.has_value());
  EXPECT_EQ(u32be->to_scalar_unchecked(), 0x1F680);

  // Test to_utf32_le
  auto u32le = to_utf32_le(*utf8);
  ASSERT_TRUE(u32le.has_value());
  EXPECT_EQ(u32le->to_scalar_unchecked(), 0x1F680);

  // Test to_utf8
  auto back_to_u8 = to_utf8(*u32be);
  ASSERT_TRUE(back_to_u8.has_value());
  EXPECT_EQ(back_to_u8->to_scalar_unchecked(), 0x1F680);
}

TEST(Endianness, ByteOrderDifference) {
  // Test that BE and LE actually produce different byte sequences for multi-byte values
  uint32_t test_value = 0x1234;  // Value that will show endianness difference

  auto utf16be = Utf16BECodePoint::from_scalar(test_value);
  auto utf16le = Utf16LECodePoint::from_scalar(test_value);
  auto utf32be = Utf32BECodePoint::from_scalar(test_value);
  auto utf32le = Utf32LECodePoint::from_scalar(test_value);

  ASSERT_TRUE(utf16be.has_value());
  ASSERT_TRUE(utf16le.has_value());
  ASSERT_TRUE(utf32be.has_value());
  ASSERT_TRUE(utf32le.has_value());

  // All should decode to the same scalar value
  EXPECT_EQ(utf16be->to_scalar_unchecked(), test_value);
  EXPECT_EQ(utf16le->to_scalar_unchecked(), test_value);
  EXPECT_EQ(utf32be->to_scalar_unchecked(), test_value);
  EXPECT_EQ(utf32le->to_scalar_unchecked(), test_value);

  // But their raw bytes should be different (on little-endian host)
  auto be_units16 = utf16be->units();
  auto le_units16 = utf16le->units();
  auto be_units32 = utf32be->units();
  auto le_units32 = utf32le->units();

  // Verify they have the same logical content but potentially different byte representation
  EXPECT_EQ(be_units16.size(), 1u);
  EXPECT_EQ(le_units16.size(), 1u);
  EXPECT_EQ(be_units32.size(), 1u);
  EXPECT_EQ(le_units32.size(), 1u);

  // Test that conversion between endiannesses preserves the scalar value
  auto be_to_le = convert<Utf16LECodePoint>(*utf16be);
  auto le_to_be = convert<Utf16BECodePoint>(*utf16le);

  ASSERT_TRUE(be_to_le.has_value());
  ASSERT_TRUE(le_to_be.has_value());
  EXPECT_EQ(be_to_le->to_scalar_unchecked(), test_value);
  EXPECT_EQ(le_to_be->to_scalar_unchecked(), test_value);
}
