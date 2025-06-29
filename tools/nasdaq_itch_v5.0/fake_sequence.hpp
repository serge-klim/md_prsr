#pragma once
#include "md_prsr/nasdaq/moldudp64_v1.0/messages.hpp"
#include "md_prsr/nasdaq/moldudp64_v1.0/describe.hpp"
#include <transcoder/transcoder.hpp>
#include <chrono>

class fake_sequence  {
 public:
    template<typename F>
    std::ptrdiff_t operator()(tc::byte_t const* begin, tc::byte_t const* end, F&& proc);
 private:
    nasdaq::moldudp64::v1_0::seqn_t seqn_ = 1;
};

template <typename F>
std::ptrdiff_t fake_sequence::operator()(tc::byte_t const* begin, tc::byte_t const* end, F&& proc) {
   static constexpr auto header_size = tc::encoded_sizeof<nasdaq::moldudp64::v1_0::packet_block, tc::effective_options<nasdaq::moldudp64::v1_0::packet_block>>::value;
   while (header_size <= static_cast<std::size_t>(std::distance(begin, end)) /*&& seqn_ < 1000000*/) {
      auto packet_block = tc::decode<nasdaq::moldudp64::v1_0::packet_block>(begin, end);
      if (packet_block.message_size > std::distance(begin, end)) {
         begin -= header_size;
         break;
      }
      auto e = begin + packet_block.message_size;
      proc(seqn_, begin, e);
      ++seqn_;
      begin = e;
   }
   return std::distance(begin, end);
}
