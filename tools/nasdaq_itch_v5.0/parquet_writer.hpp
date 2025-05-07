#pragma once
#include "md_prsr/parquet.hpp"
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include "md_prsr/nasdaq/moldudp64_v1.0/messages.hpp"
#include <boost/mp11/list.hpp>
#include <boost/mp11/algorithm.hpp>
#include <boost/program_options.hpp>
#include <tuple>
#include <chrono>

template<typename Message>
using sequenced_message_writer = pqx::writer<nasdaq::moldudp64::v1_0::seqn_t, Message>;

class parquet_writer {
   using tuple = boost::mp11::mp_rename<nasdaq::itch::v5_0::messages, std::tuple>;
   using message_writers = boost::mp11::mp_transform<sequenced_message_writer, tuple>;

 public:
   parquet_writer(boost::program_options::variables_map const& options);
   std::ptrdiff_t operator()(nasdaq::moldudp64::v1_0::seqn_t seqn, tc::byte_t const* begin, tc::byte_t const* end);
   void done(std::chrono::system_clock::duration duration);

 private:
   // std::filesystem path_;
   // H5::H5File file_;
   message_writers writers_;
   std::size_t count_ = 0;
};
