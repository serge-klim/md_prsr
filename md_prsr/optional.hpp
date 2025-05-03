#pragma once
#include "transcoder/traits.hpp"
#include <type_traits>

#include <optional>

namespace md_prsr { inline namespace v1 {

template <typename T, T Null>
class optional {
 public:
   using value_type = T;
   static constexpr value_type null_value = Null;
   optional() = default;
   optional(T value) : value_{std::move(value)} {}
   optional(std::optional<T> value) : value_{value ? *std::move(value) : null_value} {}
   constexpr T const& raw_value() const noexcept { return value_; }
   constexpr bool has_value() const noexcept { return value_ != null_value; }
   explicit constexpr operator bool() const noexcept { return has_value(); }
   operator std::optional<T>() const { return has_value() ? std::make_optional(value_) : std::nullopt; }
 private:
   T value_ = null_value;
};

}} // namespace sbe

template<typename T, T Null, typename Options> 
struct tc::encoded_sizeof<md_prsr::v1::optional<T, Null>, Options, std::true_type> : tc::encoded_sizeof<T, Options> {};

template <typename T, T Null, typename Options>
struct tc::decoder<md_prsr::v1::optional<T, Null>, Options, std::true_type> : private tc::decoder<T, Options> {
	md_prsr::v1::optional<T, Null> operator()(byte_t const*& begin, [[maybe_unused]] byte_t const* end) noexcept {
      return {tc::decoder<T, Options>::operator()(begin, end)};
	}
};

template <typename T, T Null, typename Options>
struct tc::encoder<md_prsr::v1::optional<T, Null>, Options, std::true_type> : private tc::encoder<T, Options> {
   byte_t* operator()(md_prsr::v1::optional<T, Null> const& value, byte_t* begin, [[maybe_unused]] byte_t* end) noexcept {
      return {tc::encoder<T, Options>::operator()(value.raw_value(), begin, end)};
   }
};

