#pragma once
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include "md_prsr/nasdaq/moldudp64_v1.0/messages.hpp"
#include <unordered_map>
#include <chrono>

class message_counter  {
public:
    constexpr auto const& messages() const noexcept { return messages_; }
    std::ptrdiff_t operator()(nasdaq::moldudp64::v1_0::seqn_t seqn, tc::byte_t const* begin, tc::byte_t const* end);
    void done(std::chrono::system_clock::duration duration);
private:
    struct stats {
       std::size_t n;
       nasdaq::moldudp64::v1_0::seqn_t first;
       nasdaq::moldudp64::v1_0::seqn_t last;
    };
    std::unordered_map<decltype(nasdaq::itch::v5_0::system_event::message_type), stats> messages_;
};

