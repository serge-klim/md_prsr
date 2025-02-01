#pragma once
#include "../timestamp.hpp"
#include "transcoder/traits.hpp"
//#include "transcoder/detail/options.hpp"
#include "transcoder/transcoder.hpp"
#include <boost/endian.hpp>
#include <array>
//#include <chrono>

namespace nasdaq::itch::v5_0::detail {
using raw_timestamp_t = std::array<std::uint8_t, 6>;
} // namespace nasdaq::itch::v5_0::detail

template <typename Options>
struct tc::encoded_sizeof<nasdaq::itch::v5_0::timestamp_t, Options, std::true_type>
    : tc::encoded_sizeof<nasdaq::itch::v5_0::detail::raw_timestamp_t, Options> {};

template <typename Options>
struct tc::decoder<nasdaq::itch::v5_0::timestamp_t, Options, std::true_type> {
   nasdaq::itch::v5_0::timestamp_t operator()(byte_t const*& begin, [[maybe_unused]] byte_t const* end) noexcept {
      auto raw = tc::decoder<nasdaq::itch::v5_0::detail::raw_timestamp_t, Options>{}(begin, end);
      auto value = decltype(nasdaq::itch::v5_0::timestamp_t::value){0};
      std::memcpy(reinterpret_cast<char*>(&value) + 2, &raw, ssize(raw));
      boost::endian::big_to_native_inplace(value);
      return nasdaq::itch::v5_0::timestamp_t{value};
   }
};

template <typename Options>
struct tc::encoder<nasdaq::itch::v5_0::timestamp_t, Options, std::true_type> {
   byte_t* operator()(nasdaq::itch::v5_0::timestamp_t const& ts, byte_t* begin, [[maybe_unused]] byte_t* end) noexcept {
      auto raw = nasdaq::itch::v5_0::detail::raw_timestamp_t{};
      auto value = boost::endian::native_to_big(ts.value);
      std::memcpy(&raw, reinterpret_cast<char const*>(&value) + 2, ssize(raw));
      return tc::encoder<nasdaq::itch::v5_0::detail::raw_timestamp_t, Options>{}(raw, begin, end);
   }
};

//namespace nasdaq::itch::v5_0 {
//
//} // namespace nasdaq::itch::v5_0
