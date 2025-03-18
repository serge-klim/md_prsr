#pragma once
#include "transcoder/options.hpp"
#include "transcoder/byte_order.hpp"

namespace nasdaq::itch::v5_0 {
struct tag;
using options = tc::options<tc::flag::big_endian, tag>;
options protocol_options(...);

} // namespace nasdaq::itch::v5_0
