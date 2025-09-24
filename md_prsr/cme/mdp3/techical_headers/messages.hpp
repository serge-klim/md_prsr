#pragma once
#include <cstdint>
#include "transcoder/byte_order.hpp"
#include "transcoder/options.hpp"

namespace cme::mdp3 { inline namespace v1_0 {

// https://cmegroupclientsite.atlassian.net/wiki/spaces/EPICSANDBOX/pages/457638617/MDP+3.0+-+SBE+Technical+Headers

struct packet_header {
   std::uint32_t msg_seq_num;
   std::uint64_t sending_time;
};

tc::options<tc::flag::little_endian> protocol_options(...);

}} // namespace cme::mdp3::v1_0
