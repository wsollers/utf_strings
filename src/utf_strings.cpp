
#include "../include/utf/utf_strings.hpp"

// Explicit template instantiations for commonly used types
// This ensures symbols are available when linking against the static/shared library

namespace utf {

// UTF-8 (endian doesn't matter, but we use big for consistency)
template class basic_utf_string<char8_t, endian::big>;

// UTF-16 both endians
template class basic_utf_string<char16_t, endian::big>;
template class basic_utf_string<char16_t, endian::little>;

// UTF-32 both endians
template class basic_utf_string<char32_t, endian::big>;
template class basic_utf_string<char32_t, endian::little>;

// Explicit instantiation of commonly used free functions
template std::optional<std::size_t> length<char8_t, endian::big>(
    const std::basic_string_view<char8_t>&) noexcept;
template std::optional<std::u32string> to_u32<char8_t, endian::big>(
    const std::basic_string_view<char8_t>&);

template std::optional<std::size_t> length<char16_t, endian::big>(
    const std::basic_string_view<char16_t>&) noexcept;
template std::optional<std::u32string> to_u32<char16_t, endian::big>(
    const std::basic_string_view<char16_t>&);

template std::optional<std::size_t> length<char16_t, endian::little>(
    const std::basic_string_view<char16_t>&) noexcept;
template std::optional<std::u32string> to_u32<char16_t, endian::little>(
    const std::basic_string_view<char16_t>&);

template std::optional<std::size_t> length<char32_t, endian::big>(
    const std::basic_string_view<char32_t>&) noexcept;
template std::optional<std::u32string> to_u32<char32_t, endian::big>(
    const std::basic_string_view<char32_t>&);

template std::optional<std::size_t> length<char32_t, endian::little>(
    const std::basic_string_view<char32_t>&) noexcept;
template std::optional<std::u32string> to_u32<char32_t, endian::little>(
    const std::basic_string_view<char32_t>&);

}  // namespace utf
