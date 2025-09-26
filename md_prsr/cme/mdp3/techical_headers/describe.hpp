#pragma once
#include "messages.hpp"
#include <boost/describe.hpp>

namespace cme::mdp3 { inline namespace v1_0 {

BOOST_DESCRIBE_STRUCT(packet_header, (),(
                          msg_seq_num,
                          sending_time))

}} // namespace cme::mdp3::v1_0
