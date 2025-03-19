#include "hdf5_writer.hpp"
#include "loggers.hpp"
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include "transcoder/type_name.hpp"
#include "h5x/timestamp.hpp"
#include "h5x/h5_describe.hpp"
#ifdef USE_BLOSC
#include "blosc_filter.h"
#endif // USE_BLOSC
#include <boost/mp11/utility.hpp>
#include <variant>
#include <string>
#include <type_traits>

namespace {

std::string h5_filename(boost::program_options::variables_map const& options) {
   auto const filename = options["filename"].as<std::string>();
   return filename + ".h5";
}

template <typename... T>
std::tuple<h5x::data_sink<T>...> make_message_writers(H5::H5Location const& location, boost::mp11::mp_identity<std::tuple<h5x::data_sink<T>...>>) {
#ifdef USE_BLOSC
   char* version = nullptr;
   char* date = nullptr;
   int r = register_blosc(&version, &date);
   free(version);
   free(date);
   if (r < 0)
      throw std::runtime_error{"unable to register blosc"};

   H5::DSetCreatPropList props;
   hsize_t const chunk_size = 102400;
   props.setChunk(1, &chunk_size);
   unsigned int cd_values[7] = {0};
   // https://github.com/Blosc/hdf5-blosc/blob/master/src/example.c#L81
   /* 0 to 3 (inclusive) param slots are reserved. */
   cd_values[4] = 4;             /* compression level */
   cd_values[5] = 1;             /* 0: shuffle not active, 1: shuffle active */
   cd_values[6] = BLOSC_BLOSCLZ; /* the actual compressor to use */
   props.setFilter(FILTER_BLOSC, H5Z_FLAG_OPTIONAL, 7, cd_values);

   return {h5x::data_sink<T>{location, nasdaq::itch::v5_0::type_name<T>{}().c_str(), props}...};
#else
   return {h5x::data_sink<T>{location, nasdaq::itch::v5_0::type_name<T>{}().c_str()}...};
#endif // USE_BLOSC
}

} // namespace

hdf5_writer::hdf5_writer(boost::program_options::variables_map const& options)
    : filename_{h5_filename(options)}, file_{filename_, H5F_ACC_TRUNC}, writers_{make_message_writers(file_, boost::mp11::mp_identity<message_writers>{})} {
}

std::ptrdiff_t hdf5_writer::operator()(nasdaq::moldudp64::v1_0::seqn_t /*seqn*/, tc::byte_t const* begin, tc::byte_t const* end) {
    auto const decoded = nasdaq::itch::v5_0::decode<nasdaq::itch::v5_0::messages>(begin, end);
    std::visit([this](auto message) {
        auto& writer = std::get<h5x::data_sink<decltype(message)>>(writers_);
        writer.push_back(std::move(message));
    },
                decoded);
    ++count_;
    assert(begin == end);
    return 0;
}

void hdf5_writer::done(std::chrono::system_clock::duration duration) {
   BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << "--\n"
                                                        << count_ << " messages has been written to \"" << filename_ << "\" in "
                                                        << std::chrono::duration_cast<std::chrono::duration<double, typename std::chrono::minutes::period>>(duration).count() << " mins. ("
                                                        << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << " secs.)\n";
}
