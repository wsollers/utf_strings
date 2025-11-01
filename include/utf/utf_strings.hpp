#pragma once
#include <string>
#include <string_view>
#include <cstdint>
#include <vector>
#include <optional>
#include <type_traits>
#include <bit>
#include <utility>
#include "../src/export.hpp"

namespace utf {
enum class endian { big, little, native };
constexpr bool is_native(endian e) noexcept {
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    constexpr bool host_big = true;
#else
    constexpr bool host_big = (std::endian::native == std::endian::big);
#endif
    return (e == endian::native) || (host_big && e == endian::big) || (!host_big && e == endian::little);
}
constexpr std::uint16_t swap16(std::uint16_t v) noexcept { return std::byteswap(v); }
constexpr std::uint32_t swap32(std::uint32_t v) noexcept { return std::byteswap(v); }
inline std::uint16_t load_u16(std::uint16_t v, endian e) noexcept { return is_native(e) ? v : swap16(v); }
inline std::uint32_t load_u32(std::uint32_t v, endian e) noexcept { return is_native(e) ? v : swap32(v); }

template <class Unit, endian E = endian::big> struct encoding_traits;

template <endian E>
struct encoding_traits<char8_t, E> {
    static inline bool decode_one(const char8_t* p, std::size_t n, char32_t& out, unsigned& consumed) noexcept {
        if (!n) return false;
        auto c0 = static_cast<unsigned char>(p[0]);
        if (c0 < 0x80u) { out = c0; consumed = 1; return true; }
        unsigned need = (c0>>5)==0x6?2u:((c0>>4)==0xE?3u:((c0>>3)==0x1E?4u:0u));
        if (!need || n < need) return false;
        auto c1 = static_cast<unsigned char>(p[1]); if ((c1&0xC0u)!=0x80u) return false;
        if (need==2){ unsigned u=((c0&0x1Fu)<<6)|(c1&0x3Fu); if(u<0x80u) return false; out=u; consumed=2; return true; }
        auto c2 = static_cast<unsigned char>(p[2]); if ((c2&0xC0u)!=0x80u) return false;
        if (need==3){ unsigned u=((c0&0x0Fu)<<12)|((c1&0x3Fu)<<6)|(c2&0x3Fu); if(u<0x800u) return false; if(u>=0xD800u && u<=0xDFFFu) return false; out=u; consumed=3; return true; }
        auto c3 = static_cast<unsigned char>(p[3]); if ((c3&0xC0u)!=0x80u) return false;
        unsigned u=((c0&0x07u)<<18)|((c1&0x3Fu)<<12)|((c2&0x3Fu)<<6)|(c3&0x3Fu);
        if (u<0x10000u || u>0x10FFFFu) return false; out=u; consumed=4; return true;
    }
};

template <endian E>
struct encoding_traits<char16_t, E> {
    static inline bool decode_one(const char16_t* p, std::size_t n, char32_t& out, unsigned& consumed) noexcept {
        if (!n) return false;
        std::uint16_t w1 = load_u16(static_cast<std::uint16_t>(p[0]), E);
        if (w1 < 0xD800 || w1 > 0xDFFF) { out=w1; consumed=1; return true; }
        if (w1 > 0xDBFF || n<2) return false;
        std::uint16_t w2 = load_u16(static_cast<std::uint16_t>(p[1]), E);
        if (w2 < 0xDC00 || w2 > 0xDFFF) return false;
        unsigned u = 0x10000 + (((unsigned(w1)-0xD800)<<10) | (unsigned(w2)-0xDC00));
        if (u > 0x10FFFF) return false; out=u; consumed=2; return true;
    }
};

template <endian E>
struct encoding_traits<char32_t, E> {
    static inline bool decode_one(const char32_t* p, std::size_t n, char32_t& out, unsigned& consumed) noexcept {
        if (!n) return false;
        std::uint32_t v = load_u32(static_cast<std::uint32_t>(p[0]), E);
        if (v>0x10FFFF || (v>=0xD800 && v<=0xDFFF)) return false; out=v; consumed=1; return true;
    }
};

struct CodePointSpan { std::size_t unit_offset{}, unit_length{}; };

template <class Unit, endian E = endian::big>
class UTF_STRINGS_API basic_utf_string {
    using storage_type = std::basic_string<Unit>;
public:
    basic_utf_string() = default;
    explicit basic_utf_string(storage_type s): data_(std::move(s)) {}
    explicit basic_utf_string(std::basic_string_view<Unit> v): data_(v.begin(), v.end()) {}
    bool valid() const noexcept {
        const Unit* p = data_.data(); std::size_t n = data_.size();
        while (n) { char32_t cp{}; unsigned c{}; if(!encoding_traits<Unit,E>::decode_one(p,n,cp,c)) return false; p+=c; n-=c; }
        return true;
    }
    std::optional<std::size_t> length() const noexcept {
        const Unit* p = data_.data(); std::size_t n = data_.size(), cnt=0;
        while (n) { char32_t cp{}; unsigned c{}; if(!encoding_traits<Unit,E>::decode_one(p,n,cp,c)) return std::nullopt; ++cnt; p+=c; n-=c; }
        return cnt;
    }
    std::optional<std::u32string> to_u32() const {
        const Unit* p = data_.data(); std::size_t n = data_.size(); std::u32string out; out.reserve(n);
        while (n) { char32_t cp{}; unsigned c{}; if(!encoding_traits<Unit,E>::decode_one(p,n,cp,c)) return std::nullopt; out.push_back(cp); p+=c; n-=c; }
        return out;
    }
private:
    storage_type data_{};
};

using utf8_string   = basic_utf_string<char8_t,  endian::big>;
using utf16_string  = basic_utf_string<char16_t, endian::big>;
using utf32_string  = basic_utf_string<char32_t, endian::big>;
using utf16be_string = basic_utf_string<char16_t, endian::big>;
using utf16le_string = basic_utf_string<char16_t, endian::little>;
using utf16ne_string = basic_utf_string<char16_t, endian::native>;
using utf32be_string = basic_utf_string<char32_t, endian::big>;
using utf32le_string = basic_utf_string<char32_t, endian::little>;
using utf32ne_string = basic_utf_string<char32_t, endian::native>;

template <class Unit, endian E>
inline bool valid(std::basic_string_view<Unit> v) noexcept { return basic_utf_string<Unit,E>(v).valid(); }
template <class Unit, endian E>
inline std::optional<std::size_t> length(std::basic_string_view<Unit> v) noexcept { return basic_utf_string<Unit,E>(v).length(); }
template <class Unit, endian E>
inline std::optional<std::u32string> to_u32(std::basic_string_view<Unit> v) { return basic_utf_string<Unit,E>(v).to_u32(); }
} // namespace utf
