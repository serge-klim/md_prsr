#pragma once
#include "traits.hpp"
#include "message_id.hpp"
#include "options.hpp"
#include "messages.hpp"
#include "describe.hpp"
#include "timestamp.hpp"
#include "transcoder/timestamp.hpp"
#include "transcoder/flyweight.hpp"
#include "transcoder/byte_order.hpp"
#include "transcoder/transcoder.hpp"
#include "transcoder/type_name.hpp"

namespace nasdaq::itch::v5_0 {

template <typename T>
using flyweight = tc::flyweight<T, options>;

template <typename T>
tc::byte_t *encode(T const &value, tc::byte_t *begin, tc::byte_t *end) { return tc::wrapped::encode<options, T>(value, begin, end); }

template<typename T>
[[nodiscard]] std::vector<tc::byte_t> encode(T const& value) { return tc::wrapped::encode<options, T>(value); }

template<typename T>
[[nodiscard]] T decode(tc::byte_t*& begin, tc::byte_t const* end) { return tc::wrapped::decode<T, options>(begin, end); }

template<typename T>
[[nodiscard]] T decode(tc::byte_t const*& begin, tc::byte_t const* end) { return tc::wrapped::decode<T, options>(begin, end); }



using tc::type_name;

} // namespace nasdaq::itch::v5_0



