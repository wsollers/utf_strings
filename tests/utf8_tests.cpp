#include <gtest/gtest.h>
#include <string>
#include "utf/utf_strings.hpp"
using namespace utf;
TEST(UTF8, ValidAscii){ utf8_string s{std::u8string{u8"hello"}}; EXPECT_TRUE(s.valid()); EXPECT_EQ(*s.length(), 5u); }
TEST(UTF8, ValidMultibyte){ utf8_string s{std::u8string{u8"Héllø 🌍"}}; ASSERT_TRUE(s.valid()); EXPECT_EQ(*s.length(), 8u); }
TEST(UTF16BE, Surrogate){ std::u16string nat{0xD83C,0xDF0D}; auto be=utf16be_string::from_native(nat); EXPECT_TRUE(be.valid()); EXPECT_EQ(*be.length(),1u); }
