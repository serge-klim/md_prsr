#pragma once
#include "messages.hpp"
#include <boost/describe.hpp>

namespace nasdaq::moldudp64::v1_0 {

BOOST_DESCRIBE_STRUCT(header, (),
(
    session,
    sequence_number,
    message_count,
))
   
BOOST_DESCRIBE_STRUCT(packet_block, (),
(
    message_size;
))

} // namespace nasdaq::itch::v5_1

