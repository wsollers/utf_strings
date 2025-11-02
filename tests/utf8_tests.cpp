
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
  utf8_string s{std::u8string{u8"Héllø 🌍"}};
  ASSERT_TRUE(s.valid());
  auto n = s.length();
  ASSERT_TRUE(n.has_value());
  EXPECT_EQ(*n, 8u);
  auto u32 = s.to_u32();
  ASSERT_TRUE(u32.has_value());
  EXPECT_EQ((*u32)[0], U'H');
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
  EXPECT_EQ((*u32)[0], U'🌍');
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
  std::u32string nat{U'H', U'é', U'ø', U'🌍'};
  auto be = utf32be_string::from_native(nat);
  EXPECT_TRUE(be.valid());
  EXPECT_EQ(*be.length(), 4u);
  auto round = be.to_native();
  EXPECT_EQ(round, nat);
}
