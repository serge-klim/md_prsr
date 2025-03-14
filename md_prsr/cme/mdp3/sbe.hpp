#pragma once
#include "transcoder/traits.hpp"
#include <type_traits>

//#include "types.hpp"
#include <optional>
#include <tuple>
#include <vector>

namespace sbe { inline namespace v1 {

template <typename T, T Null>
class optional
{
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


template <typename T, typename GroupSize>
struct group
{
  std::vector<T> data;
};

}} // namespace sbe

//#include "sizeof.hpp"
template<typename T, T Null, typename Options> 
struct tc::encoded_sizeof<sbe::v1::optional<T, Null>, Options, std::true_type> : tc::encoded_sizeof<T, Options> {};

template <typename T, typename GroupSize, typename Options>
struct tc::encoded_sizeof<sbe::v1::group<T, GroupSize>, Options, std::true_type> {
    using type = tc::dynamic_size; 
    std::size_t operator()(sbe::v1::group<T, GroupSize> const& value) const {
       return tc::encoded_sizeof<GroupSize, Options>::value + tc::encoded_sizeof<T, Options>{}() * value.data.size();
    }
};


//#include "decoder.hpp"
#include <vector>
#include <iterator>
#include <stdexcept>

template <typename T, T Null, typename Options>
struct tc::decoder<sbe::v1::optional<T, Null>, Options, std::true_type> : private tc::decoder<T, Options>
{
	sbe::v1::optional<T, Null> operator()(byte_t const*& begin, [[maybe_unused]] byte_t const* end) noexcept {
      return {tc::decoder<T, Options>::operator()(begin, end)};
	}
};

template <typename T, typename GroupSize, typename Options>
struct tc::decoder<sbe::v1::group<T, GroupSize>, Options, std::true_type> //: private tc::decoder<T, Options>
{
   sbe::v1::group<T, GroupSize> operator()(byte_t const*& begin, [[maybe_unused]] byte_t const* end)
   {
      if (std::distance(begin, end) < tc::encoded_sizeof<GroupSize, Options>::value)
         throw std::length_error{"not enough data provided to decode sbe::group"};
      auto group_size = tc::decoder<GroupSize, Options>{}(begin, end);
      if (std::distance(begin, end) < group_size.blockLength * group_size.numInGroup)
         throw std::length_error{"not enough data provided to decode sbe::group"};

      auto items = std::vector<T>{};
      items.reserve(group_size.numInGroup);
      while (group_size.numInGroup--)
         items.push_back(tc::decoder<T, Options>{}(begin, end));
      return {items};
   }
};

// #include "encoder.hpp"
#include <vector>
#include <iterator>
#include <stdexcept>

template <typename T, T Null, typename Options>
struct tc::encoder<sbe::v1::optional<T, Null>, Options, std::true_type> : private tc::encoder<T, Options> {
   byte_t* operator()(sbe::v1::optional<T, Null> const& value, byte_t* begin, [[maybe_unused]] byte_t* end) noexcept {
      return {tc::encoder<T, Options>::operator()(value.raw_value(), begin, end)};
   }
};

template <typename T, typename GroupSize, typename Options>
struct tc::encoder<sbe::v1::group<T, GroupSize>, Options, std::true_type> //: private tc::decoder<T, Options>
{
   byte_t* operator()(sbe::v1::group<T,GroupSize> const& value, byte_t* begin, [[maybe_unused]] byte_t* end) {
      if (std::distance(begin, end) < tc::encoded_sizeof<GroupSize, Options>::value)
         throw std::length_error{"not enough space to encode sbe::group to"};
      auto group_size = GroupSize{};
      //TDOD:blockLength has to be set!!!!!!!!!
      group_size.numInGroup = static_cast<decltype(group_size.numInGroup)>(value.data.size());
      begin = tc::encoder<GroupSize, Options>{}(group_size, begin, end);
      for (auto const& item :value.data)
         // TODO:check lenght and throw for dynamic_size maybe?
         begin = tc::encoder<T, Options>{}(item, begin, end);
      return begin;
   }
};
