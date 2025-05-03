#pragma once
#include "md_prsr/optional.hpp"
#include "transcoder/traits.hpp"
#include <type_traits>

//#include "types.hpp"
#include <optional>
#include <tuple>
#include <vector>

namespace sbe { inline namespace v1 {

template <typename T, T Null>
using optional = md_prsr::optional<T,Null>;


template <typename T, typename GroupSize>
struct group
{
  std::vector<T> data;
};

}} // namespace sbe

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
