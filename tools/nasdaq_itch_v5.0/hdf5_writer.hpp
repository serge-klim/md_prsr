#pragma once
#include "h5x/data_sink.hpp"
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include <boost/mp11/list.hpp>
#include <boost/mp11/algorithm.hpp>
#include <boost/program_options.hpp>
#include <tuple>
#include <chrono>

class hdf5_writer  {
    using tuple = boost::mp11::mp_rename<nasdaq::itch::v5_0::messages, std::tuple>; 
    using message_writers = boost::mp11::mp_transform<h5x::data_sink, tuple>;
  public:
    hdf5_writer(boost::program_options::variables_map const &options);
    std::ptrdiff_t operator()(tc::byte_t const* begin, tc::byte_t const* end);
    void done(std::chrono::system_clock::duration duration);
  private:
    std::string filename_;
    H5::H5File file_;
    message_writers writers_;
    std::size_t count_ = 0;
};

