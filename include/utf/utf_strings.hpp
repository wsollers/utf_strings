#pragma once

// utf_string.hpp - Modern C++23 UTF String Library
//
// A type-safe, constexpr-enabled library for handling UTF-8, UTF-16, and UTF-32
// strings with explicit endianness control. Built on top of utf_codepoint.hpp.
//
// Features:
// - UTF-8/16/32 string views and containers
// - Small String Optimization (SSO) - strings <= 32 bytes stored on stack
// - Iterator support for code point traversal
// - String conversion between encodings (explicit constructors and assignment)
// - String concatenation operators
// - Validation and sanitization
// - constexpr and noexcept throughout for zero runtime overhead
// - Range-based operations
//
// Requirements:
// - C++23 or later
// - utf_codepoint.hpp (included automatically)
// - Standard library support for: <string>, <string_view>, <ranges>
//
// Example Usage:
//   // Create UTF-8 string view
//   utf::string::Utf8StringView sv{u8"Hello 世界 💩"};
//
//   // Iterate over code points
//   for (auto cp : sv) {
//       // cp is utf::Utf8CodePoint
//       if (cp.is_valid()) {
//           auto scalar = cp.to_scalar();
//           // ...
//       }
//   }
//
//   // Convert between encodings (now with direct assignment!)
//   utf::string::Utf32BEString u32{0x1F4A9};
//   utf::string::Utf8String u8{u32};  // Converting constructor
//   u8 = u32;                         // Converting assignment
//
//   // String concatenation
//   auto combined = u8 + u8;
//   u8 += utf::Utf8CodePoint{0x21};  // Append '!'
//
// SPDX-License-Identifier: MIT

#ifndef UTF_STRING_HPP
#define UTF_STRING_HPP

#define UTF_STRING_VERSION_MAJOR 1
#define UTF_STRING_VERSION_MINOR 3
#define UTF_STRING_VERSION_PATCH 0

// Require C++23
#if __cplusplus < 202302L
#error "UTF String library requires C++23 or later"
#endif

#include <algorithm>
#include <compare>
#include <cstring>
#include <iterator>
#include <limits>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "utf_codepoints.hpp"

namespace utf::string {

// ============================================================================
// Forward Declarations
// ============================================================================

template <typename UtfType, Endian E = Endian::BE>
  requires ValidEndianness<UtfType, E>
class StringView;

template <typename UtfType, Endian E = Endian::BE>
  requires ValidEndianness<UtfType, E>
class String;

// ============================================================================
// Iterator for UTF Strings
// ============================================================================

/// @brief Iterator for traversing UTF-encoded strings as code points
/// @tparam UtfType The UTF encoding type (Utf8, Utf16, or Utf32)
/// @tparam E The endianness (Endian::None for UTF-8, BE or LE for UTF-16/32)
template <typename UtfType, Endian E>
  requires ValidEndianness<UtfType, E>
class CodePointIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = CodePoint<UtfType, E>;
  using difference_type = std::ptrdiff_t;
  using pointer = const value_type*;
  using reference = value_type;

  constexpr CodePointIterator() noexcept = default;

  constexpr CodePointIterator(const typename UtfType::storage_type* ptr,
                              const typename UtfType::storage_type* end) noexcept
      : current_(ptr), end_(end) {
    if (current_ != end_) {
      read_current();
    }
  }

  [[nodiscard]] constexpr reference operator*() const noexcept { return current_code_point_; }

  [[nodiscard]] constexpr pointer operator->() const noexcept { return &current_code_point_; }

  constexpr CodePointIterator& operator++() noexcept {
    advance();
    return *this;
  }

  constexpr CodePointIterator operator++(int) noexcept {
    CodePointIterator tmp = *this;
    advance();
    return tmp;
  }

  [[nodiscard]] constexpr bool operator==(const CodePointIterator& other) const noexcept {
    return current_ == other.current_;
  }

  [[nodiscard]] constexpr bool operator!=(const CodePointIterator& other) const noexcept {
    return current_ != other.current_;
  }

  /// @brief Get the current position in the underlying buffer
  [[nodiscard]] constexpr const typename UtfType::storage_type* position() const noexcept {
    return current_;
  }

 private:
  const typename UtfType::storage_type* current_{nullptr};
  const typename UtfType::storage_type* end_{nullptr};
  value_type current_code_point_{};

  // Helper function to do endian conversion - works around private access
  static constexpr typename UtfType::storage_type to_native_endian(
      typename UtfType::storage_type v) noexcept {
    if constexpr (ByteOriented<UtfType>) {
      return v;  // UTF-8 has no endianness
    } else {
      // Use std::byteswap when needed
      if constexpr ((E == Endian::LE && std::endian::native == std::endian::big) ||
                    (E == Endian::BE && std::endian::native == std::endian::little)) {
        return std::byteswap(v);
      } else {
        return v;
      }
    }
  }

  constexpr void read_current() noexcept {
    if (current_ >= end_) {
      return;
    }

    if constexpr (ByteOriented<UtfType>) {
      // UTF-8: Read based on lead byte
      std::size_t remaining = end_ - current_;
      if (remaining == 0) return;

      // Determine how many bytes needed
      uint8_t lead = *current_;
      std::size_t needed = 1;
      if ((lead & 0x80) == 0x00)
        needed = 1;
      else if ((lead & 0xE0) == 0xC0)
        needed = 2;
      else if ((lead & 0xF0) == 0xE0)
        needed = 3;
      else if ((lead & 0xF8) == 0xF0)
        needed = 4;
      else {
        // Invalid lead byte - create invalid code point
        current_code_point_.rune[0] = 0xFF;
        return;
      }

      if (needed > remaining) {
        // Truncated sequence - mark as invalid
        current_code_point_.rune[0] = 0xFF;
        return;
      }

      // Copy bytes
      for (std::size_t i = 0; i < needed && i < 4; ++i) {
        current_code_point_.rune[i] = current_[i];
      }

    } else if constexpr (std::same_as<UtfType, Utf16>) {
      // UTF-16: Read 1 or 2 units
      std::size_t remaining = end_ - current_;
      if (remaining == 0) return;

      current_code_point_.rune[0] = *current_;

      // Check if this is a high surrogate (needs second unit)
      uint16_t first = to_native_endian(*current_);
      if (first >= 0xD800 && first <= 0xDBFF) {
        // High surrogate - need low surrogate
        if (remaining >= 2) {
          current_code_point_.rune[1] = current_[1];
        } else {
          // Truncated - mark as invalid (high surrogate + high surrogate)
          typename UtfType::storage_type invalid_marker = 0xD800;
          if constexpr ((E == Endian::LE && std::endian::native == std::endian::big) ||
                        (E == Endian::BE && std::endian::native == std::endian::little)) {
            invalid_marker = std::byteswap(invalid_marker);
          }
          current_code_point_.rune[0] = invalid_marker;
          current_code_point_.rune[1] = invalid_marker;
        }
      } else {
        current_code_point_.rune[1] = 0;
      }

    } else {  // UTF-32
      current_code_point_.rune = *current_;
    }
  }

  constexpr void advance() noexcept {
    if (current_ >= end_) return;

    std::size_t advance_by = current_code_point_.count();
    if (advance_by == 0) advance_by = 1;  // Skip invalid byte/unit

    current_ += advance_by;

    if (current_ < end_) {
      read_current();
    }
  }
};

// ============================================================================
// UTF String View
// ============================================================================

/// @brief Non-owning view of a UTF-encoded string
/// @tparam UtfType The UTF encoding type (Utf8, Utf16, or Utf32)
/// @tparam E The endianness (Endian::None for UTF-8, BE or LE for UTF-16/32)
template <typename UtfType, Endian E>
  requires ValidEndianness<UtfType, E>
class StringView {
 public:
  using value_type = CodePoint<UtfType, E>;
  using size_type = std::size_t;
  using storage_type = typename UtfType::storage_type;
  using iterator = CodePointIterator<UtfType, E>;
  using const_iterator = iterator;
  using string_type = String<UtfType, E>;

  /// @brief Default constructor creates an empty view
  constexpr StringView() noexcept = default;

  /// @brief Construct from pointer and length (in storage units)
  constexpr StringView(const storage_type* data, size_type length) noexcept
      : data_(data), length_(length) {}

  /// @brief Construct from null-terminated string
  /// @note Computes length by scanning for null terminator
  /// @warning String must be null-terminated
  constexpr explicit StringView(const storage_type* data) noexcept
      : data_(data), length_(compute_length(data)) {}

  /// @brief Construct from std::basic_string
  template <typename Traits, typename Allocator>
  constexpr StringView(const std::basic_string<storage_type, Traits, Allocator>& str) noexcept
      : data_(str.data()), length_(str.size()) {}

  /// @brief Construct from std::basic_string_view
  template <typename Traits>
  constexpr StringView(std::basic_string_view<storage_type, Traits> sv) noexcept
      : data_(sv.data()), length_(sv.size()) {}

  /// @brief Get pointer to the underlying data
  [[nodiscard]] constexpr const storage_type* data() const noexcept { return data_; }

  /// @brief Get the length in storage units (not code points!)
  [[nodiscard]] constexpr size_type length() const noexcept { return length_; }

  /// @brief Get the size in storage units (alias for length())
  [[nodiscard]] constexpr size_type size() const noexcept { return length_; }

  /// @brief Get the size in bytes
  [[nodiscard]] constexpr size_type size_bytes() const noexcept {
    return length_ * sizeof(storage_type);
  }

  /// @brief Check if the view is empty
  [[nodiscard]] constexpr bool empty() const noexcept { return length_ == 0; }

  /// @brief Get iterator to the beginning
  [[nodiscard]] constexpr iterator begin() const noexcept {
    return iterator(data_, data_ + length_);
  }

  /// @brief Get iterator to the end
  [[nodiscard]] constexpr iterator end() const noexcept {
    return iterator(data_ + length_, data_ + length_);
  }

  /// @brief Count the number of code points in the string
  /// @note This iterates through the string, O(n) complexity
  [[nodiscard]] constexpr size_type count_code_points() const noexcept {
    size_type count = 0;
    for ([[maybe_unused]] auto cp : *this) {
      ++count;
    }
    return count;
  }

  /// @brief Validate the entire string
  /// @return true if all code points are valid
  [[nodiscard]] constexpr bool is_valid() const noexcept {
    for (auto cp : *this) {
      if (!cp.is_valid()) {
        return false;
      }
    }
    return true;
  }

  /// @brief Convert to std::basic_string_view
  [[nodiscard]] constexpr std::basic_string_view<storage_type> to_std_string_view() const noexcept {
    return std::basic_string_view<storage_type>(data_, length_);
  }

  /// @brief Create a substring view
  [[nodiscard]] constexpr StringView substr(
      size_type pos, size_type count = std::string_view::npos) const noexcept {
    if (pos >= length_) {
      return StringView();
    }
    size_type actual_count =
        (count == std::string_view::npos) ? (length_ - pos) : std::min(count, length_ - pos);
    return StringView(data_ + pos, actual_count);
  }

  /// @brief Equality comparison
  [[nodiscard]] constexpr bool operator==(const StringView& other) const noexcept {
    if (length_ != other.length_) return false;
    return std::memcmp(data_, other.data_, length_ * sizeof(storage_type)) == 0;
  }

  /// @brief Three-way comparison
  [[nodiscard]] constexpr std::strong_ordering operator<=>(const StringView& other) const noexcept {
    size_type min_len = std::min(length_, other.length_);
    int cmp = std::memcmp(data_, other.data_, min_len * sizeof(storage_type));
    if (cmp != 0) return cmp <=> 0;
    return length_ <=> other.length_;
  }

 private:
  const storage_type* data_{nullptr};
  size_type length_{0};

  static constexpr size_type compute_length(const storage_type* str) noexcept {
    if (!str) return 0;
    size_type len = 0;
    // Note: Assumes null-terminated string. Reading past end is UB.
    while (str[len] != storage_type{0}) {
      ++len;
    }
    return len;
  }
};

// ============================================================================
// Small String Buffer (SSO Implementation)
// ============================================================================

/// @brief Small buffer optimization for UTF strings
/// @details Strings up to 32 bytes total (including metadata) are stored inline on the stack.
///          Actual data capacity is 32 - 2*sizeof(size_t) bytes.
/// @note StorageType must be trivially copyable (enforced by static_assert)
template <typename StorageType>
class SmallStringBuffer {
  // Enforce trivially copyable requirement for memcpy safety
  static_assert(std::is_trivially_copyable_v<StorageType>,
                "StorageType must be trivially copyable for SSO");

 public:
  // Calculate capacity to keep total size at 32 bytes
  // Layout: [size_t size] [size_t capacity] [bool is_inline] [union: inline_data or heap_data*]
  static constexpr std::size_t total_size = 32;
  static constexpr std::size_t metadata_size =
      sizeof(std::size_t) + sizeof(std::size_t) + sizeof(bool);
  static constexpr std::size_t inline_capacity = (total_size - metadata_size) / sizeof(StorageType);

  // Constructor - properly initialize union
  SmallStringBuffer() noexcept
      : size_(0), capacity_(inline_capacity), is_inline_(true), inline_data_{} {}

  // Destructor - clean up heap allocation if needed
  ~SmallStringBuffer() noexcept {
    if (!is_inline_ && heap_data_) {
      delete[] heap_data_;
    }
  }

  // Copy constructor
  SmallStringBuffer(const SmallStringBuffer& other)
      : size_(other.size_), capacity_(other.capacity_), is_inline_(other.is_inline_) {
    if (is_inline_) {
      std::memcpy(inline_data_, other.inline_data_, size_ * sizeof(StorageType));
    } else {
      heap_data_ = new StorageType[capacity_];
      std::memcpy(heap_data_, other.heap_data_, size_ * sizeof(StorageType));
    }
  }

  // Move constructor
  SmallStringBuffer(SmallStringBuffer&& other) noexcept
      : size_(other.size_), capacity_(other.capacity_), is_inline_(other.is_inline_) {
    if (is_inline_) {
      std::memcpy(inline_data_, other.inline_data_, size_ * sizeof(StorageType));
    } else {
      heap_data_ = other.heap_data_;
      other.heap_data_ = nullptr;
      other.is_inline_ = true;
      other.size_ = 0;
      other.capacity_ = inline_capacity;
    }
  }

  // Copy assignment
  SmallStringBuffer& operator=(const SmallStringBuffer& other) {
    if (this != &other) {
      SmallStringBuffer tmp(other);
      swap(tmp);
    }
    return *this;
  }

  // Move assignment
  SmallStringBuffer& operator=(SmallStringBuffer&& other) noexcept {
    if (this != &other) {
      swap(other);
    }
    return *this;
  }

  [[nodiscard]] constexpr const StorageType* data() const noexcept {
    return is_inline_ ? inline_data_ : heap_data_;
  }

  [[nodiscard]] constexpr StorageType* data() noexcept {
    return is_inline_ ? inline_data_ : heap_data_;
  }

  [[nodiscard]] constexpr std::size_t size() const noexcept { return size_; }

  [[nodiscard]] constexpr std::size_t capacity() const noexcept { return capacity_; }

  [[nodiscard]] constexpr bool is_inline() const noexcept { return is_inline_; }

  constexpr void clear() noexcept { size_ = 0; }

  void reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity_) return;

    // Check for overflow
    if (new_capacity > std::numeric_limits<std::size_t>::max() / sizeof(StorageType)) {
      throw std::length_error("Requested capacity would overflow");
    }

    // Allocate new buffer
    StorageType* new_data = new StorageType[new_capacity];

    // Copy existing data
    if (size_ > 0) {
      std::memcpy(new_data, data(), size_ * sizeof(StorageType));
    }

    // Clean up old heap data if necessary
    if (!is_inline_ && heap_data_) {
      delete[] heap_data_;
    }

    // Switch to heap
    heap_data_ = new_data;
    capacity_ = new_capacity;
    is_inline_ = false;
  }

  void push_back(StorageType value) {
    if (size_ >= capacity_) {
      // Check for overflow before doubling
      std::size_t new_cap = capacity_ * 2;
      if (new_cap < capacity_) {  // Overflow occurred
        throw std::length_error("Capacity overflow");
      }
      reserve(new_cap);
    }
    data()[size_++] = value;
  }

  void append(const StorageType* src, std::size_t count) {
    // Validate input
    if (count > 0 && !src) {
      throw std::invalid_argument("Null source pointer with non-zero count");
    }

    // Check for size overflow
    if (count > std::numeric_limits<std::size_t>::max() - size_) {
      throw std::length_error("String size would overflow");
    }

    std::size_t new_size = size_ + count;
    if (new_size > capacity_) {
      std::size_t new_cap = std::max(capacity_ * 2, new_size);
      // Check for overflow in capacity calculation
      if (new_cap < new_size) {
        throw std::length_error("Capacity overflow");
      }
      reserve(new_cap);
    }

    if (count > 0) {
      std::memcpy(data() + size_, src, count * sizeof(StorageType));
      size_ = new_size;
    }
  }

  void swap(SmallStringBuffer& other) noexcept {
    if (is_inline_ && other.is_inline_) {
      // Both inline - swap inline buffers
      StorageType tmp_data[inline_capacity];
      std::size_t tmp_size = size_;

      std::memcpy(tmp_data, inline_data_, size_ * sizeof(StorageType));
      std::memcpy(inline_data_, other.inline_data_, other.size_ * sizeof(StorageType));
      std::memcpy(other.inline_data_, tmp_data, tmp_size * sizeof(StorageType));

      size_ = other.size_;
      other.size_ = tmp_size;
    } else if (!is_inline_ && !other.is_inline_) {
      // Both heap - swap pointers
      std::swap(heap_data_, other.heap_data_);
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
    } else {
      // One inline, one heap - complex swap
      if (is_inline_) {
        // *this is inline, other is heap
        StorageType inline_copy[inline_capacity];
        std::memcpy(inline_copy, inline_data_, size_ * sizeof(StorageType));
        std::size_t size_copy = size_;

        heap_data_ = other.heap_data_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        is_inline_ = false;

        std::memcpy(other.inline_data_, inline_copy, size_copy * sizeof(StorageType));
        other.size_ = size_copy;
        other.capacity_ = inline_capacity;
        other.is_inline_ = true;
        other.heap_data_ = nullptr;
      } else {
        // *this is heap, other is inline
        other.swap(*this);
      }
    }
  }

 private:
  std::size_t size_{0};
  std::size_t capacity_{inline_capacity};
  bool is_inline_{true};

  union {
    StorageType inline_data_[inline_capacity];
    StorageType* heap_data_;
  };
};

// ============================================================================
// UTF String (Owning Container with SSO)
// ============================================================================

/// @brief Owning container for UTF-encoded strings with Small String Optimization
/// @tparam UtfType The UTF encoding type (Utf8, Utf16, or Utf32)
/// @tparam E The endianness (Endian::None for UTF-8, BE or LE for UTF-16/32)
/// @details Total size is 32 bytes. Inline capacities:
///          - UTF-8: ~16 units (exact: (32 - 17) / 1)
///          - UTF-16: ~7 units (exact: (32 - 17) / 2)
///          - UTF-32: ~3 units (exact: (32 - 17) / 4)
///          Strings exceeding inline capacity are heap-allocated.
/// @note Iterator Invalidation: append(), reserve(), operator+=, operator+ may invalidate iterators
/// @note Exception Safety: Strong guarantee for copy operations, basic guarantee for modifications
template <typename UtfType, Endian E>
  requires ValidEndianness<UtfType, E>
class String {
 public:
  using value_type = CodePoint<UtfType, E>;
  using size_type = std::size_t;
  using storage_type = typename UtfType::storage_type;
  using iterator = CodePointIterator<UtfType, E>;
  using const_iterator = iterator;
  using view_type = StringView<UtfType, E>;

  /// @brief Default constructor creates an empty string
  String() noexcept = default;

  /// @brief Construct from a view
  String(view_type view) { buffer_.append(view.data(), view.length()); }

  /// @brief Construct from pointer and length
  String(const storage_type* data, size_type length) { buffer_.append(data, length); }

  /// @brief Construct from null-terminated string
  /// @warning String must be null-terminated
  explicit String(const storage_type* str) {
    if (str) {
      size_type len = 0;
      while (str[len] != storage_type{0}) ++len;
      buffer_.append(str, len);
    }
  }

  /// @brief Construct from std::basic_string
  template <typename Traits, typename Allocator>
  explicit String(const std::basic_string<storage_type, Traits, Allocator>& str) {
    buffer_.append(str.data(), str.size());
  }

  /// @brief Construct from initializer list of code points
  String(std::initializer_list<value_type> code_points) {
    for (const auto& cp : code_points) {
      append(cp);
    }
  }

  /// @brief Converting constructor from different encoding
  /// @tparam SrcUtfType Source UTF encoding type
  /// @tparam SrcEndian Source endianness
  /// @param other String in different encoding to convert from
  /// @throws std::invalid_argument if source contains invalid code points
  /// @note Explicit to prevent accidental conversions
  template <typename SrcUtfType, Endian SrcEndian>
    requires ValidEndianness<SrcUtfType, SrcEndian> &&
             (!std::same_as<SrcUtfType, UtfType> || SrcEndian != E)
  explicit String(const String<SrcUtfType, SrcEndian>& other) {
    assign_from(other);
  }

  /// @brief Get pointer to the underlying data
  [[nodiscard]] constexpr const storage_type* data() const noexcept { return buffer_.data(); }

  /// @brief Get mutable pointer to the underlying data
  [[nodiscard]] constexpr storage_type* data() noexcept { return buffer_.data(); }

  /// @brief Get the length in storage units
  [[nodiscard]] constexpr size_type length() const noexcept { return buffer_.size(); }

  /// @brief Get the size in storage units (alias for length())
  [[nodiscard]] constexpr size_type size() const noexcept { return buffer_.size(); }

  /// @brief Get the size in bytes
  [[nodiscard]] constexpr size_type size_bytes() const noexcept {
    return buffer_.size() * sizeof(storage_type);
  }

  /// @brief Check if the string is empty
  [[nodiscard]] constexpr bool empty() const noexcept { return buffer_.size() == 0; }

  /// @brief Check if string is stored inline (on stack)
  [[nodiscard]] constexpr bool is_inline() const noexcept { return buffer_.is_inline(); }

  /// @brief Get the inline capacity (maximum size before heap allocation)
  [[nodiscard]] static constexpr size_type inline_capacity() noexcept {
    return SmallStringBuffer<storage_type>::inline_capacity;
  }

  /// @brief Clear the string (does not deallocate heap memory)
  constexpr void clear() noexcept { buffer_.clear(); }

  /// @brief Reserve capacity for storage units
  /// @note May invalidate iterators
  void reserve(size_type capacity) { buffer_.reserve(capacity); }

  /// @brief Get the capacity in storage units
  [[nodiscard]] constexpr size_type capacity() const noexcept { return buffer_.capacity(); }

  /// @brief Append a code point
  /// @note May invalidate iterators
  void append(const value_type& cp) {
    auto units = cp.units();
    buffer_.append(units.data(), units.size());
  }

  /// @brief Append a string view
  /// @note May invalidate iterators
  void append(view_type view) { buffer_.append(view.data(), view.length()); }

  /// @brief Append operator for code point
  /// @note May invalidate iterators
  String& operator+=(const value_type& cp) {
    append(cp);
    return *this;
  }

  /// @brief Append operator for string view
  /// @note May invalidate iterators
  String& operator+=(view_type view) {
    append(view);
    return *this;
  }

  /// @brief Append operator for string
  /// @note May invalidate iterators
  String& operator+=(const String& other) {
    append(other.view());
    return *this;
  }

  /// @brief Get iterator to the beginning
  [[nodiscard]] constexpr iterator begin() const noexcept {
    return iterator(buffer_.data(), buffer_.data() + buffer_.size());
  }

  /// @brief Get iterator to the end
  [[nodiscard]] constexpr iterator end() const noexcept {
    return iterator(buffer_.data() + buffer_.size(), buffer_.data() + buffer_.size());
  }

  /// @brief Convert to view
  [[nodiscard]] constexpr operator view_type() const noexcept {
    return view_type(buffer_.data(), buffer_.size());
  }

  /// @brief Get a view of this string
  [[nodiscard]] constexpr view_type view() const noexcept {
    return view_type(buffer_.data(), buffer_.size());
  }

  /// @brief Count the number of code points
  [[nodiscard]] constexpr size_type count_code_points() const noexcept {
    return view().count_code_points();
  }

  /// @brief Validate the entire string
  [[nodiscard]] constexpr bool is_valid() const noexcept { return view().is_valid(); }

  /// @brief Convert to std::basic_string
  [[nodiscard]] std::basic_string<storage_type> to_std_string() const {
    return std::basic_string<storage_type>(buffer_.data(), buffer_.data() + buffer_.size());
  }

  // ============================================================================
  // Factory Methods for Creating Strings from Byte Arrays
  // ============================================================================

  /// @brief Create string from raw byte array assuming correct byte order
  /// @param bytes Pointer to byte array containing UTF data already in the encoding's expected byte
  /// order
  /// @param byte_count Number of bytes in the array
  /// @return Optional string created from the byte data, or nullopt if invalid UTF sequences found
  /// @note Assumes bytes are already in the correct byte order for the encoding (BE/LE)
  /// @note For UTF-8: bytes are used as-is but validated
  /// @note For UTF-16/32: bytes are assumed to be in the correct endianness already but validated
  [[nodiscard]] static std::optional<String> from_bytes(const uint8_t* bytes, size_t byte_count) {
    if (!bytes) {
      return String{};
    }

    // Ensure proper alignment for multi-byte encodings
    if constexpr (std::is_same_v<UtfType, encodings::Utf16>) {
      if (byte_count % 2 != 0) {
        return std::nullopt;  // Invalid alignment
      }
    } else if constexpr (std::is_same_v<UtfType, encodings::Utf32>) {
      if (byte_count % 4 != 0) {
        return std::nullopt;  // Invalid alignment
      }
    }

    String result;

    if constexpr (std::is_same_v<UtfType, encodings::Utf8>) {
      // UTF-8: direct byte interpretation but validate
      result.buffer_.append(reinterpret_cast<const storage_type*>(bytes), byte_count);
    } else {
      // UTF-16/32: assume bytes are already in correct byte order, just cast them
      const size_t unit_count = byte_count / sizeof(storage_type);
      result.buffer_.reserve(unit_count);

      // Direct reinterpret cast - assumes bytes are in correct order already
      const storage_type* units = reinterpret_cast<const storage_type*>(bytes);
      result.buffer_.append(units, unit_count);
    }

    // Validate the resulting string
    if (!result.is_valid()) {
      return std::nullopt;
    }

    return result;
  }

  /// @brief Create string from std::vector<uint8_t> byte array
  /// @param bytes Vector containing UTF data in the encoding's expected format
  /// @return Optional string created from the byte data, or nullopt if invalid UTF sequences found
  [[nodiscard]] static std::optional<String> from_bytes(const std::vector<uint8_t>& bytes) {
    return from_bytes(bytes.data(), bytes.size());
  }

  /// @brief Create string from std::array<uint8_t> byte array
  /// @param bytes Array containing UTF data in the encoding's expected format
  /// @return Optional string created from the byte data, or nullopt if invalid UTF sequences found
  template <size_t N>
  [[nodiscard]] static std::optional<String> from_bytes(const std::array<uint8_t, N>& bytes) {
    return from_bytes(bytes.data(), N);
  }

  /// @brief Swap with another string
  void swap(String& other) noexcept { buffer_.swap(other.buffer_); }

  /// @brief Equality comparison
  [[nodiscard]] constexpr bool operator==(const String& other) const noexcept {
    return view() == other.view();
  }

  /// @brief Equality comparison with view
  [[nodiscard]] constexpr bool operator==(view_type other) const noexcept {
    return view() == other;
  }

  /// @brief Three-way comparison
  [[nodiscard]] constexpr std::strong_ordering operator<=>(const String& other) const noexcept {
    return view() <=> other.view();
  }

  /// @brief Converting assignment from different encoding
  /// @tparam SrcUtfType Source UTF encoding type
  /// @tparam SrcEndian Source endianness
  /// @param other String in different encoding to convert from
  /// @return Reference to this string
  /// @throws std::invalid_argument if source contains invalid code points
  /// @note May invalidate iterators
  template <typename SrcUtfType, Endian SrcEndian>
    requires ValidEndianness<SrcUtfType, SrcEndian> &&
             (!std::same_as<SrcUtfType, UtfType> || SrcEndian != E)
  String& operator=(const String<SrcUtfType, SrcEndian>& other) {
    assign_from(other);
    return *this;
  }

  /// @brief Non-throwing conversion assignment from different encoding
  /// @tparam SrcUtfType Source UTF encoding type
  /// @tparam SrcEndian Source endianness
  /// @param other String in different encoding to convert from
  /// @return true if conversion succeeded, false if source contains invalid code points
  /// @note May invalidate iterators on success
  template <typename SrcUtfType, Endian SrcEndian>
    requires ValidEndianness<SrcUtfType, SrcEndian>
  [[nodiscard]] bool try_assign_from(const String<SrcUtfType, SrcEndian>& other) noexcept {
    try {
      buffer_.clear();
      buffer_.reserve(other.length());

      for (auto cp : other) {
        auto converted = convert<value_type>(cp);
        if (!converted) {
          return false;
        }
        append(*converted);
      }
      return true;
    } catch (...) {
      return false;
    }
  }

 private:
  SmallStringBuffer<storage_type> buffer_;

  /// @brief Helper method for conversion (used by constructor and assignment)
  /// @note Optimized to avoid intermediate string allocation
  template <typename SrcUtfType, Endian SrcEndian>
  void assign_from(const String<SrcUtfType, SrcEndian>& other) {
    buffer_.clear();
    buffer_.reserve(other.length());

    for (auto cp : other) {
      auto converted = convert<value_type>(cp);
      if (!converted) {
        throw std::invalid_argument("Invalid UTF code point in source string");
      }
      append(*converted);
    }
  }
};

// ============================================================================
// String Concatenation Operators
// ============================================================================

/// @brief Concatenate two strings of the same encoding
template <typename UtfType, Endian E>
  requires ValidEndianness<UtfType, E>
[[nodiscard]] String<UtfType, E> operator+(const String<UtfType, E>& lhs,
                                           const String<UtfType, E>& rhs) {
  String<UtfType, E> result;
  result.reserve(lhs.length() + rhs.length());
  result.append(lhs.view());
  result.append(rhs.view());
  return result;
}

/// @brief Concatenate string with string view
template <typename UtfType, Endian E>
  requires ValidEndianness<UtfType, E>
[[nodiscard]] String<UtfType, E> operator+(const String<UtfType, E>& lhs,
                                           StringView<UtfType, E> rhs) {
  String<UtfType, E> result;
  result.reserve(lhs.length() + rhs.length());
  result.append(lhs.view());
  result.append(rhs);
  return result;
}

/// @brief Concatenate string view with string
template <typename UtfType, Endian E>
  requires ValidEndianness<UtfType, E>
[[nodiscard]] String<UtfType, E> operator+(StringView<UtfType, E> lhs,
                                           const String<UtfType, E>& rhs) {
  String<UtfType, E> result;
  result.reserve(lhs.length() + rhs.length());
  result.append(lhs);
  result.append(rhs.view());
  return result;
}

/// @brief Concatenate string with code point
template <typename UtfType, Endian E>
  requires ValidEndianness<UtfType, E>
[[nodiscard]] String<UtfType, E> operator+(const String<UtfType, E>& lhs,
                                           const CodePoint<UtfType, E>& rhs) {
  String<UtfType, E> result{lhs};
  result.append(rhs);
  return result;
}

/// @brief Concatenate code point with string
template <typename UtfType, Endian E>
  requires ValidEndianness<UtfType, E>
[[nodiscard]] String<UtfType, E> operator+(const CodePoint<UtfType, E>& lhs,
                                           const String<UtfType, E>& rhs) {
  String<UtfType, E> result;
  result.reserve(rhs.length() + 4);  // Rough estimate
  result.append(lhs);
  result.append(rhs.view());
  return result;
}

// ============================================================================
// Type Aliases
// ============================================================================

// String Views
using Utf8StringView = StringView<Utf8, Endian::None>;
using Utf16BEStringView = StringView<Utf16, Endian::BE>;
using Utf16LEStringView = StringView<Utf16, Endian::LE>;
using Utf32BEStringView = StringView<Utf32, Endian::BE>;
using Utf32LEStringView = StringView<Utf32, Endian::LE>;

// Owning Strings (with SSO)
using Utf8String = String<Utf8, Endian::None>;
using Utf16BEString = String<Utf16, Endian::BE>;
using Utf16LEString = String<Utf16, Endian::LE>;
using Utf32BEString = String<Utf32, Endian::BE>;
using Utf32LEString = String<Utf32, Endian::LE>;

// ============================================================================
// String Conversion Functions
// ============================================================================

/// @brief Convert a UTF string to a different encoding
/// @tparam DestString The destination string type
/// @tparam SrcUtfType The source UTF encoding type
/// @tparam SrcEndian The source endianness
/// @param source The source string view
/// @return The converted string, or std::nullopt if any code point is invalid
template <typename DestString, typename SrcUtfType, Endian SrcEndian>
  requires ValidEndianness<SrcUtfType, SrcEndian>
[[nodiscard]] std::optional<DestString> convert_string(StringView<SrcUtfType, SrcEndian> source) {
  DestString result;
  result.reserve(source.length());  // Rough estimate

  for (auto cp : source) {
    auto converted = convert<typename DestString::value_type>(cp);
    if (!converted) {
      return std::nullopt;  // Invalid code point encountered
    }
    result.append(*converted);
  }

  return result;
}

/// @brief Convert a UTF string without validation (fast path)
/// @warning All code points in source must be valid
template <typename DestString, typename SrcUtfType, Endian SrcEndian>
  requires ValidEndianness<SrcUtfType, SrcEndian>
[[nodiscard]] DestString convert_string_unchecked(StringView<SrcUtfType, SrcEndian> source) {
  DestString result;
  result.reserve(source.length());

  for (auto cp : source) {
    auto converted = convert_unchecked<typename DestString::value_type>(cp);
    result.append(converted);
  }

  return result;
}

// ============================================================================
// Convenience Conversion Functions
// ============================================================================

/// @brief Convert any UTF string to UTF-8
template <typename SrcUtfType, Endian SrcEndian>
[[nodiscard]] std::optional<Utf8String> to_utf8_string(StringView<SrcUtfType, SrcEndian> source) {
  return convert_string<Utf8String>(source);
}

/// @brief Convert any UTF string to UTF-16 BE
template <typename SrcUtfType, Endian SrcEndian>
[[nodiscard]] std::optional<Utf16BEString> to_utf16_be_string(
    StringView<SrcUtfType, SrcEndian> source) {
  return convert_string<Utf16BEString>(source);
}

/// @brief Convert any UTF string to UTF-16 LE
template <typename SrcUtfType, Endian SrcEndian>
[[nodiscard]] std::optional<Utf16LEString> to_utf16_le_string(
    StringView<SrcUtfType, SrcEndian> source) {
  return convert_string<Utf16LEString>(source);
}

/// @brief Convert any UTF string to UTF-32 BE
template <typename SrcUtfType, Endian SrcEndian>
[[nodiscard]] std::optional<Utf32BEString> to_utf32_be_string(
    StringView<SrcUtfType, SrcEndian> source) {
  return convert_string<Utf32BEString>(source);
}

/// @brief Convert any UTF string to UTF-32 LE
template <typename SrcUtfType, Endian SrcEndian>
[[nodiscard]] std::optional<Utf32LEString> to_utf32_le_string(
    StringView<SrcUtfType, SrcEndian> source) {
  return convert_string<Utf32LEString>(source);
}

// ============================================================================
// Convenience Factory Functions for Creating Strings from Bytes
// ============================================================================

/// @brief Create UTF-8 string from byte array
/// @param bytes Pointer to UTF-8 encoded bytes
/// @param byte_count Number of bytes
/// @return Optional UTF-8 string, or nullopt if invalid UTF-8 sequences found
[[nodiscard]] inline std::optional<Utf8String> utf8_string_from_bytes(const uint8_t* bytes,
                                                                      size_t byte_count) {
  return Utf8String::from_bytes(bytes, byte_count);
}

/// @brief Create UTF-8 string from byte vector
/// @param bytes Vector containing UTF-8 encoded bytes
/// @return Optional UTF-8 string, or nullopt if invalid UTF-8 sequences found
[[nodiscard]] inline std::optional<Utf8String> utf8_string_from_bytes(
    const std::vector<uint8_t>& bytes) {
  return Utf8String::from_bytes(bytes);
}

/// @brief Create UTF-16 BE string from byte array
/// @param bytes Pointer to UTF-16 BE encoded bytes
/// @param byte_count Number of bytes (must be even)
/// @return Optional UTF-16 BE string, or nullopt if invalid UTF-16 sequences found
[[nodiscard]] inline std::optional<Utf16BEString> utf16_be_string_from_bytes(const uint8_t* bytes,
                                                                             size_t byte_count) {
  return Utf16BEString::from_bytes(bytes, byte_count);
}

/// @brief Create UTF-16 BE string from byte vector
/// @param bytes Vector containing UTF-16 BE encoded bytes
/// @return Optional UTF-16 BE string, or nullopt if invalid UTF-16 sequences found
[[nodiscard]] inline std::optional<Utf16BEString> utf16_be_string_from_bytes(
    const std::vector<uint8_t>& bytes) {
  return Utf16BEString::from_bytes(bytes);
}

/// @brief Create UTF-16 LE string from byte array
/// @param bytes Pointer to UTF-16 LE encoded bytes
/// @param byte_count Number of bytes (must be even)
/// @return Optional UTF-16 LE string, or nullopt if invalid UTF-16 sequences found
[[nodiscard]] inline std::optional<Utf16LEString> utf16_le_string_from_bytes(const uint8_t* bytes,
                                                                             size_t byte_count) {
  return Utf16LEString::from_bytes(bytes, byte_count);
}

/// @brief Create UTF-16 LE string from byte vector
/// @param bytes Vector containing UTF-16 LE encoded bytes
/// @return Optional UTF-16 LE string, or nullopt if invalid UTF-16 sequences found
[[nodiscard]] inline std::optional<Utf16LEString> utf16_le_string_from_bytes(
    const std::vector<uint8_t>& bytes) {
  return Utf16LEString::from_bytes(bytes);
}

/// @brief Create UTF-32 BE string from byte array
/// @param bytes Pointer to UTF-32 BE encoded bytes
/// @param byte_count Number of bytes (must be multiple of 4)
/// @return Optional UTF-32 BE string, or nullopt if invalid UTF-32 sequences found
[[nodiscard]] inline std::optional<Utf32BEString> utf32_be_string_from_bytes(const uint8_t* bytes,
                                                                             size_t byte_count) {
  return Utf32BEString::from_bytes(bytes, byte_count);
}

/// @brief Create UTF-32 BE string from byte vector
/// @param bytes Vector containing UTF-32 BE encoded bytes
/// @return Optional UTF-32 BE string, or nullopt if invalid UTF-32 sequences found
[[nodiscard]] inline std::optional<Utf32BEString> utf32_be_string_from_bytes(
    const std::vector<uint8_t>& bytes) {
  return Utf32BEString::from_bytes(bytes);
}

/// @brief Create UTF-32 LE string from byte array
/// @param bytes Pointer to UTF-32 LE encoded bytes
/// @param byte_count Number of bytes (must be multiple of 4)
/// @return Optional UTF-32 LE string, or nullopt if invalid UTF-32 sequences found
[[nodiscard]] inline std::optional<Utf32LEString> utf32_le_string_from_bytes(const uint8_t* bytes,
                                                                             size_t byte_count) {
  return Utf32LEString::from_bytes(bytes, byte_count);
}

/// @brief Create UTF-32 LE string from byte vector
/// @param bytes Vector containing UTF-32 LE encoded bytes
/// @return Optional UTF-32 LE string, or nullopt if invalid UTF-32 sequences found
[[nodiscard]] inline std::optional<Utf32LEString> utf32_le_string_from_bytes(
    const std::vector<uint8_t>& bytes) {
  return Utf32LEString::from_bytes(bytes);
}

}  // namespace utf::string

#endif  // UTF_STRING_HPP