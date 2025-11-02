
#pragma once
#include <bit>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#ifdef _MSC_VER
#include <intrin.h>
// Suppress MSVC warning C4251 for standard library types in DLL interface
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

#include "../src/export.hpp"

namespace utf {

// ---------- Endianness policy ----------
enum class endian { big, little, native };

constexpr bool is_native(endian e) noexcept {
#if defined(__cpp_lib_endian) && __cpp_lib_endian >= 201907L
  constexpr bool host_big = (std::endian::native == std::endian::big);
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  constexpr bool host_big = true;
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  constexpr bool host_big = false;
#elif defined(_MSC_VER) || defined(__i386__) || defined(__x86_64__) || defined(__amd64__)
  // Most common platforms are little-endian
  constexpr bool host_big = false;
#else
  // Fallback using runtime detection for portability
  constexpr std::uint32_t test_value = 0x01020304;
  constexpr bool host_big = (*reinterpret_cast<const std::uint8_t*>(&test_value) == 0x01);
#endif
  return (e == endian::native) || (host_big && e == endian::big) ||
         (!host_big && e == endian::little);
}

constexpr std::uint16_t swap16(std::uint16_t v) noexcept {
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
  return std::byteswap(v);
#elif defined(__GNUC__) || defined(__clang__)
  return __builtin_bswap16(v);
#elif defined(_MSC_VER)
  return _byteswap_ushort(v);
#else
  return (v << 8) | (v >> 8);
#endif
}

constexpr std::uint32_t swap32(std::uint32_t v) noexcept {
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
  return std::byteswap(v);
#elif defined(__GNUC__) || defined(__clang__)
  return __builtin_bswap32(v);
#elif defined(_MSC_VER)
  return _byteswap_ulong(v);
#else
  return ((v & 0xFF000000u) >> 24) | ((v & 0x00FF0000u) >> 8) | ((v & 0x0000FF00u) << 8) |
         ((v & 0x000000FFu) << 24);
#endif
}

inline std::uint16_t load_u16(std::uint16_t v, endian e) noexcept {
  return is_native(e) ? v : swap16(v);
}
inline std::uint32_t load_u32(std::uint32_t v, endian e) noexcept {
  return is_native(e) ? v : swap32(v);
}

// ---------- Encoding traits ----------
template <class Unit, endian E = endian::big>
struct encoding_traits;  // primary template

// UTF-8 (endianness ignored)
template <endian E>
struct encoding_traits<char8_t, E> {
  using unit_type = char8_t;
  using view_type = std::basic_string_view<unit_type>;
  static constexpr endian order = E;
  static inline bool decode_one(const unit_type* p, std::size_t n, char32_t& out,
                                unsigned& consumed) noexcept {
    (void)order;
    if (n == 0) return false;
    auto c0 = static_cast<unsigned char>(p[0]);
    if (c0 < 0x80u) {
      out = c0;
      consumed = 1;
      return true;
    }
    unsigned need = (c0 >> 5) == 0x6 ? 2u : ((c0 >> 4) == 0xE ? 3u : ((c0 >> 3) == 0x1E ? 4u : 0u));
    if (!need || n < need) return false;
    auto c1 = static_cast<unsigned char>(p[1]);
    if ((c1 & 0xC0u) != 0x80u) return false;
    if (need == 2) {
      unsigned u = ((c0 & 0x1Fu) << 6) | (c1 & 0x3Fu);
      if (u < 0x80u) return false;
      out = static_cast<char32_t>(u);
      consumed = 2;
      return true;
    }
    auto c2 = static_cast<unsigned char>(p[2]);
    if ((c2 & 0xC0u) != 0x80u) return false;
    if (need == 3) {
      unsigned u = ((c0 & 0x0Fu) << 12) | ((c1 & 0x3Fu) << 6) | (c2 & 0x3Fu);
      if (u < 0x800u) return false;
      if (u >= 0xD800u && u <= 0xDFFFu) return false;
      out = static_cast<char32_t>(u);
      consumed = 3;
      return true;
    }
    auto c3 = static_cast<unsigned char>(p[3]);
    if ((c3 & 0xC0u) != 0x80u) return false;
    unsigned u = ((c0 & 0x07u) << 18) | ((c1 & 0x3Fu) << 12) | ((c2 & 0x3Fu) << 6) | (c3 & 0x3Fu);
    if (u < 0x10000u || u > 0x10FFFFu) return false;
    out = static_cast<char32_t>(u);
    consumed = 4;
    return true;
  }
};

// UTF-16
template <endian E>
struct encoding_traits<char16_t, E> {
  using unit_type = char16_t;
  using view_type = std::basic_string_view<unit_type>;
  static constexpr endian order = E;
  static inline bool decode_one(const unit_type* p, std::size_t n, char32_t& out,
                                unsigned& consumed) noexcept {
    if (n == 0) return false;
    std::uint16_t w1 = load_u16(static_cast<std::uint16_t>(p[0]), order);
    if (w1 < 0xD800 || w1 > 0xDFFF) {
      out = w1;
      consumed = 1;
      return true;
    }
    if (w1 > 0xDBFF || n < 2) return false;
    std::uint16_t w2 = load_u16(static_cast<std::uint16_t>(p[1]), order);
    if (w2 < 0xDC00 || w2 > 0xDFFF) return false;
    unsigned u = 0x10000 + (((static_cast<unsigned>(w1) - 0xD800) << 10) |
                            (static_cast<unsigned>(w2) - 0xDC00));
    if (u > 0x10FFFF) return false;
    out = static_cast<char32_t>(u);
    consumed = 2;
    return true;
  }
};

// UTF-32
template <endian E>
struct encoding_traits<char32_t, E> {
  using unit_type = char32_t;
  using view_type = std::basic_string_view<unit_type>;
  static constexpr endian order = E;
  static inline bool decode_one(const unit_type* p, std::size_t n, char32_t& out,
                                unsigned& consumed) noexcept {
    if (n == 0) return false;
    std::uint32_t v = load_u32(static_cast<std::uint32_t>(p[0]), order);
    if (v > 0x10FFFF || (v >= 0xD800 && v <= 0xDFFF)) return false;
    out = static_cast<char32_t>(v);
    consumed = 1;
    return true;
  }
};

// ---------- Public API ----------
struct CodePointSpan {
  std::size_t unit_offset{};
  std::size_t unit_length{};
};
struct DecodeError {
  std::size_t unit_offset{};
};

template <class Unit, endian E = endian::big>
class UTF_STRINGS_API basic_utf_string {
 public:
  using unit_type = Unit;
  using storage_type = std::basic_string<unit_type>;
  using view_type = std::basic_string_view<unit_type>;
  static constexpr endian order = E;

  basic_utf_string() = default;
  explicit basic_utf_string(storage_type s) : data_(std::move(s)) {}
  explicit basic_utf_string(view_type v) : data_(v.begin(), v.end()) {}

  [[nodiscard]] view_type view() const noexcept { return view_type{data_.data(), data_.size()}; }
  [[nodiscard]] const storage_type& str() const noexcept { return data_; }

  // Convert from host-native storage into declared endian storage
  static basic_utf_string from_native(storage_type s) {
    if constexpr (std::is_same_v<Unit, char8_t>) {
      return basic_utf_string{std::move(s)};
    } else {
      if (!is_native(E)) {
        if constexpr (std::is_same_v<Unit, char16_t>) {
          for (auto& cu : s) cu = static_cast<char16_t>(swap16(static_cast<std::uint16_t>(cu)));
        } else if constexpr (std::is_same_v<Unit, char32_t>) {
          for (auto& cu : s) cu = static_cast<char32_t>(swap32(static_cast<std::uint32_t>(cu)));
        }
      }
      return basic_utf_string{std::move(s)};
    }
  }

  // Convert to host-native storage copy
  [[nodiscard]] storage_type to_native() const {
    if constexpr (std::is_same_v<Unit, char8_t>) return data_;
    storage_type out = data_;
    if (!is_native(E)) {
      if constexpr (std::is_same_v<Unit, char16_t>) {
        for (auto& cu : out) cu = static_cast<char16_t>(swap16(static_cast<std::uint16_t>(cu)));
      } else if constexpr (std::is_same_v<Unit, char32_t>) {
        for (auto& cu : out) cu = static_cast<char32_t>(swap32(static_cast<std::uint32_t>(cu)));
      }
    }
    return out;
  }

  [[nodiscard]] bool valid() const noexcept {
    const auto* p = data_.data();
    std::size_t n = data_.size();
    while (n) {
      char32_t cp{};
      unsigned consumed{};
      if (!encoding_traits<Unit, E>::decode_one(p, n, cp, consumed)) return false;
      p += consumed;
      n -= consumed;
    }
    return true;
  }

  [[nodiscard]] std::optional<std::size_t> length() const noexcept {
    const auto* p = data_.data();
    std::size_t n = data_.size();
    std::size_t count = 0;
    while (n) {
      char32_t cp{};
      unsigned consumed{};
      if (!encoding_traits<Unit, E>::decode_one(p, n, cp, consumed)) return std::nullopt;
      ++count;
      p += consumed;
      n -= consumed;
    }
    return count;
  }

  [[nodiscard]] std::optional<std::u32string> to_u32() const {
    const auto* p = data_.data();
    std::size_t n = data_.size();
    std::u32string out;
    out.reserve(n);
    while (n) {
      char32_t cp{};
      unsigned consumed{};
      if (!encoding_traits<Unit, E>::decode_one(p, n, cp, consumed)) return std::nullopt;
      out.push_back(cp);
      p += consumed;
      n -= consumed;
    }
    return out;
  }

  [[nodiscard]] std::optional<std::vector<CodePointSpan>> spans() const {
    const auto* p = data_.data();
    std::size_t n = data_.size();
    std::vector<CodePointSpan> out;
    out.reserve(n);
    std::size_t off = 0;
    while (n) {
      char32_t cp{};
      unsigned consumed{};
      if (!encoding_traits<Unit, E>::decode_one(p, n, cp, consumed)) return std::nullopt;
      out.push_back({off, consumed});
      p += consumed;
      n -= consumed;
      off += consumed;
    }
    return out;
  }

 private:
  storage_type data_{};
};

// Aliases (default to network byte order = big-endian)
using utf8_string = basic_utf_string<char8_t, endian::big>;  // endian ignored
using utf16_string = basic_utf_string<char16_t, endian::big>;
using utf32_string = basic_utf_string<char32_t, endian::big>;

using utf16be_string = basic_utf_string<char16_t, endian::big>;
using utf16le_string = basic_utf_string<char16_t, endian::little>;
using utf16ne_string = basic_utf_string<char16_t, endian::native>;

using utf32be_string = basic_utf_string<char32_t, endian::big>;
using utf32le_string = basic_utf_string<char32_t, endian::little>;
using utf32ne_string = basic_utf_string<char32_t, endian::native>;

// Convenience free functions operating on views
template <class Unit, endian E>
[[nodiscard]] inline bool valid(const std::basic_string_view<Unit>& v) noexcept {
  return basic_utf_string<Unit, E>(v).valid();
}

template <class Unit, endian E>
[[nodiscard]] inline std::optional<std::size_t> length(
    const std::basic_string_view<Unit>& v) noexcept {
  return basic_utf_string<Unit, E>(v).length();
}

template <class Unit, endian E>
[[nodiscard]] inline std::optional<std::u32string> to_u32(const std::basic_string_view<Unit>& v) {
  return basic_utf_string<Unit, E>(v).to_u32();
}

}  // namespace utf

#ifdef _MSC_VER
#pragma warning(pop)
#endif
