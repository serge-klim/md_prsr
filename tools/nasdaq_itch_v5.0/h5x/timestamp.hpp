#pragma once
#include "md_prsr/nasdaq/itch_v5.0/timestamp.hpp"
#include "h5x/h5_types.hpp"

template <>
struct h5x::h5_type<nasdaq::itch::v5_0::timestamp_t, std::true_type> : h5x::h5_type<decltype(nasdaq::itch::v5_0::timestamp_t::value)> {
};
