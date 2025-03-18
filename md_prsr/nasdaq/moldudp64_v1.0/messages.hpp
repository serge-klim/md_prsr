#pragma once
#include <array>
#include <cstdint>

namespace nasdaq::moldudp64::v1_0 {

using session_t = std::array<char,10>;
using seqn_t = std::uint64_t;


struct header
{
    session_t session;
    seqn_t sequence_number;
    std::uint16_t message_count;
};
   

struct packet_block {
    std::uint16_t message_size;
};

using request_packet = header;
   

} // namespace nasdaq::moldudp64::v1_0

