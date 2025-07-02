#pragma once
#include "transcoder/tagged_t.hpp"
#include <cstdint>
//#include <chrono>

namespace nasdaq::itch::v5_0 {

struct timestamp_tag;
using timestamp_t = tc::tagged_t<std::uint64_t, timestamp_tag>;

//using timestamp_t = std::chrono::duration<std::uint64_t,std::nano>;

} // namespace nasdaq::itch::v5_0
