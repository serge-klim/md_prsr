#include "loggers.hpp"
#include "message_counter.hpp"
#include "message_dump.hpp"
//#include "snapshotter.hpp"
#include "fake_sequence.hpp"
#include "hdf5_writer.hpp"
#include "parquet_writer.hpp"
#include <boost/program_options.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/spirit/home/x3.hpp>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <iterator>
#include <cstdint>
#include <cstdio>

template<typename Processor>
auto read_directly(boost::iostreams::filtering_streambuf<boost::iostreams::input>&& in, Processor& proc, std::size_t buffer_size) {
    auto sequence = fake_sequence{};
    auto buffer = std::vector<char>(buffer_size);
    auto begin = buffer.data();
    auto start = std::chrono::system_clock::now();
    for (auto leftover = typename decltype(buffer)::size_type{ 0 };;) {
        auto read = boost::iostreams::read(in, begin + leftover, buffer.size() - leftover);
        if (read == -1)
            break;

        auto end = begin + leftover + read;
        leftover = sequence(begin, end, proc);
        if (leftover == buffer_size)
            throw std::runtime_error{ "decoded size seems wrong, or buffer has to be increased" };
        if (leftover != 0)
            std::memmove(begin, std::prev(end, leftover), leftover);
    }
    return std::chrono::system_clock::now() - start;
}

template<typename Processor>
auto unpack_then_process(boost::iostreams::filtering_streambuf<boost::iostreams::input>&& in, Processor& proc, std::size_t buffer_size) {
    struct sentry {
        ~sentry() {
            std::error_code ec;
            if (!std::filesystem::remove(params.path, ec)) {
                BOOST_LOG_SEV(log::get(), boost::log::trivial::error) << "unable to remove temporary file \"" << params.path <<"\" : " << ec.message();
            }
        }
        boost::iostreams::mapped_file_params params;
    } s;
    s.params.path = std::tmpnam(nullptr);
    s.params.new_file_size = buffer_size;
    s.params.flags = boost::iostreams::mapped_file::mapmode::readwrite;
    boost::iostreams::mapped_file_sink mmap;
    mmap.open(s.params);
    BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << "unpacking to memfile \"" << s.params.path << "\"...";
    auto total = std::size_t{ 0 };
    auto start = std::chrono::system_clock::now();
    for (;;) {
        auto begin = mmap.data();
        auto read = boost::iostreams::read(in, begin + total, buffer_size);
        if (read == -1)
            break;
        total += read;
        if (read < static_cast<std::streamsize>(buffer_size))
            break;
        mmap.resize(total + buffer_size/4);
    }
    //mmap.resize(total);
    auto duration = std::chrono::system_clock::now() - start;
    BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << total <<" bytes, has been unpacked in : " 
        << std::chrono::duration_cast<std::chrono::duration<double, typename std::chrono::minutes::period>>(duration).count() << " mins. ("
        << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << " secs.)";
    auto begin = mmap.data();
    auto end = begin + total;
    auto sequence = fake_sequence{};
    start = std::chrono::system_clock::now();
    auto leftover = sequence(begin, end, proc);
    duration = std::chrono::system_clock::now() - start;
    if (leftover != 0) {
        BOOST_LOG_SEV(log::get(), boost::log::trivial::warning) << leftover << " bytes, was left unprocessed !!!";
    }
    return duration;
}

template<typename Processor>
auto process_raw(std::string const& filename, Processor& proc) {
    boost::iostreams::mapped_file_sink mmap;
    mmap.open(filename);
    auto sequence = fake_sequence{};
    auto start = std::chrono::system_clock::now();
    auto begin = mmap.data();
    auto end = begin + mmap.size();
    auto leftover = sequence(begin, end, proc);
    auto period = std::chrono::system_clock::now() - start;
    if (leftover != 0) {
        BOOST_LOG_SEV(log::get(), boost::log::trivial::warning) << leftover << " bytes, was left unprocessed !!!";
    }
    return period;
}

enum class input_type {
    gzip,
    raw
};

input_type check_type(std::string const& filename) {
    auto in = std::ifstream{ filename, std::ios::binary };
    auto magic = std::uint16_t{};
    in.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    return magic == 0x8b1f ? input_type::gzip : input_type::raw;
}

template <typename Processor>
void run(boost::program_options::variables_map const &options, Processor &proc) {
    auto duration = std::chrono::system_clock::duration::min();
    try {
       struct on_falure {
          on_falure(std::chrono::system_clock::duration& duration) : 
              duration_{duration}, start_{std::chrono::system_clock::now()}
          {}
          ~on_falure(){
             if (duration_ == std::chrono::system_clock::duration::min())
                duration_ = std::chrono::system_clock::now() - start_;
          }
        private:
          std::chrono::system_clock::duration& duration_;
          std::chrono::system_clock::time_point start_;
       } on_falure_{duration};
       auto const filename = options["filename"].as<std::string>();
       if (check_type(filename) == input_type::gzip) {
          BOOST_LOG_SEV(log::get(), boost::log::trivial::warning) << "processing file: " << filename << "...";
          auto in = boost::iostreams::filtering_streambuf<boost::iostreams::input>{};
          in.push(boost::iostreams::gzip_decompressor());
          auto src = boost::iostreams::stream<boost::iostreams::mapped_file_source>{filename};
          in.push(src);

          auto buffer_size = options["buffer-size"].as<std::size_t>();
          if (buffer_size < 1500)
             buffer_size = 1500;

          duration = options["unpack-first"].as<bool>()
                         ? unpack_then_process(std::move(in), proc, std::filesystem::file_size(filename) * 3)
                         : read_directly(std::move(in), proc, buffer_size);
       } else {
          BOOST_LOG_SEV(log::get(), boost::log::trivial::warning) << "processing raw file: " << filename << "...";
          duration = process_raw(filename, proc);
       }
    } catch (std::exception& e) {
       BOOST_LOG_SEV(log::get(), boost::log::trivial::error) << "error: " << e.what();
    } catch (...) {
       BOOST_LOG_SEV(log::get(), boost::log::trivial::error) << "unexpected error!";
    }
    proc.done(duration);
}

void run(boost::program_options::variables_map const &options) {
  using processor = void (*)(boost::program_options::variables_map const &);
  struct message_field_parser : boost::spirit::x3::symbols<processor> {
    message_field_parser() {
      // clang-format off
          boost::spirit::x3::symbols<processor>::add
              //("snapshot", [] (boost::program_options::variables_map const &options){
              //      auto proc = snapshotter{};
              //      run(options, proc);
              //    })
              ("count", [] (boost::program_options::variables_map const &options){
                    auto proc = message_counter{};
                    run(options, proc);
                  })
              ("dump", [] (boost::program_options::variables_map const &options){
                    auto proc = message_dumper{};
                    run(options, proc);
                  })
              ("hdf5", [] (boost::program_options::variables_map const &options){
                    auto proc = hdf5_writer{options};
                    run(options, proc);
                  })
             ("parquet", [] (boost::program_options::variables_map const &options){
                    auto proc = parquet_writer{options};
                    run(options, proc);
                  })
	       ;
      // clang-format on
    }
  } static const parser;

  auto proc = processor{};

  auto processor_type = options["run"].as<std::string>();
  auto begin = cbegin(processor_type);
  auto end = cend(processor_type);
  if (!boost::spirit::x3::parse(begin, end, parser, proc) || begin != end)
    throw boost::program_options::error{"unknown processor : " + processor_type};
  (*proc)(options);
}
