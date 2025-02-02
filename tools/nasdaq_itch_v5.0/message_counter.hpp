#pragma once
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include <unordered_map>
#include <chrono>

class message_counter  {
public:
    constexpr auto const& messages() const noexcept { return messages_; }
    std::ptrdiff_t operator()(tc::byte_t const* begin, tc::byte_t const* end);
    void done(std::chrono::system_clock::duration duration);
private:
    std::unordered_map<decltype(nasdaq::itch::v5_0::system_event::message_type), std::size_t> messages_;
};

