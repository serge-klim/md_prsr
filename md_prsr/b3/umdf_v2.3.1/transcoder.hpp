#pragma once
#include "messages.hpp"
#include "transcoder/transcoder.hpp"

 template <typename T, std::string T::* P, typename Options>
struct tc::encoded_member_sizeof<P, Options, std::true_type> /*: encoded_sizeof<std::uint64_t, Options>*/ {
   using type = tc::dynamic_size;
   std::size_t operator()(std::string const& value) const {
      return value.size();
   }
};

namespace sbe { inline namespace v1 {

template <typename Descriptor, typename Options>
struct member_string_decoder {
   std::string operator()(tc::byte_t const*& begin, [[maybe_unused]] tc::byte_t const* end) const {
      if (std::distance(begin, end) < tc::encoded_sizeof<Descriptor, Options>::value)
         throw std::length_error{"not enough data provided to decode sbe::data"};
      auto length = tc::decoder<decltype(Descriptor::length), Options>{}(begin, end);
      if (std::distance(begin, end) < length)
         throw std::length_error{"not enough data provided to decode sbe::data"};
      auto data = begin;
      begin += length;
      return {static_cast<char const*>(data), static_cast<std::string::size_type>(length)};
   }
};

}} // namespace sbe::v1

template <typename Options>
struct tc::member_decoder<&b3::umdf::v2_3_1::SecurityDefinition_12::securityDesc, Options, std::true_type>
    : sbe::member_string_decoder<b3::umdf::v2_3_1::TextEncoding, Options> {
};

template <typename Options>
struct tc::member_decoder<&b3::umdf::v2_3_1::News_5::headline, Options, std::true_type>
    : sbe::member_string_decoder<b3::umdf::v2_3_1::VarString, Options> {
};
template <typename Options>
struct tc::member_decoder<&b3::umdf::v2_3_1::News_5::text, Options, std::true_type>
    : sbe::member_string_decoder<b3::umdf::v2_3_1::VarString, Options> {
};
template <typename Options>
struct tc::member_decoder<&b3::umdf::v2_3_1::News_5::uRLLink, Options, std::true_type>
    : sbe::member_string_decoder<b3::umdf::v2_3_1::VarString, Options> {
};
