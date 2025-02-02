#pragma once
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include <chrono>

class message_dumper  {
 public:
    std::ptrdiff_t operator()(tc::byte_t const* begin, tc::byte_t const* end);
    void done(std::chrono::system_clock::duration duration);
 private:
    std::size_t count_ = 0;
};

