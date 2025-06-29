#include "parquet_writer.hpp"
#include "loggers.hpp"
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include "transcoder/type_name.hpp"
#include "arrow/io/file.h"
#include <boost/mp11/utility.hpp>
#include <variant>
#include <filesystem>
#include <string>
#include <format>
#include <stdexcept>
#include <type_traits>

namespace {
//
// std::string h5_filename(boost::program_options::variables_map const& options) {
//   auto const filename = options["filename"].as<std::string>();
//   return filename + ".h5";
//}
//
template <typename T>
std::shared_ptr<arrow::io::OutputStream> make_parquet_sink(std::filesystem::path path) {
   auto type_name = nasdaq::itch::v5_0::type_name<T>{}();
   auto pos = type_name.find_last_of(" :");
   auto file_name = pos == std::string::npos ? std::string{} : type_name.substr(pos + 1);
   file_name += ".parquet";
   path /= file_name;
   auto res = arrow::io::FileOutputStream::Open(path.string());
   if (!res.ok())
      throw std::runtime_error{std::format("unable to open file {} : {}", path.string(), res.status().message())};
   return res.ValueUnsafe();
}

template <typename... T>
std::tuple<sequenced_message_writer<T>...> make_message_writers(boost::program_options::variables_map const& options, boost::mp11::mp_identity<std::tuple<sequenced_message_writer<T>...>>) {
   parquet::WriterProperties::Builder properties_builder;
   // properties_builder.compression(parquet::Compression::LZ4);
   properties_builder.compression(parquet::Compression::SNAPPY);
   const auto properties = properties_builder.build();

   auto path = std::string{};
   if (auto const& option = options["path"]; !option.empty())
      path = option.as<std::string>();
   return {sequenced_message_writer<T>{pqx::options<pqx::flatten>{}, make_parquet_sink<T>(path), properties}...};
}
} // namespace

parquet_writer::parquet_writer(boost::program_options::variables_map const& options)
    : writers_{make_message_writers(options, boost::mp11::mp_identity<message_writers>{})} {
}

std::ptrdiff_t parquet_writer::operator()(nasdaq::moldudp64::v1_0::seqn_t seqn, tc::byte_t const* begin, tc::byte_t const* end) {
   auto const decoded = nasdaq::itch::v5_0::decode<nasdaq::itch::v5_0::messages>(begin, end);
   std::visit([this, seqn](auto const& message) {
      auto& writer = std::get<sequenced_message_writer<std::decay_t<decltype(message)>>>(writers_);
      writer.push_back(seqn, message);
   },
              decoded);
   ++count_;
   assert(begin == end);
   return std::distance(begin, end);
}

void parquet_writer::done(std::chrono::system_clock::duration duration) {
   BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << "--\n"
                                                        //<< count_ << " messages has been written to \"" << filename_ << "\" in "
                                                        << std::chrono::duration_cast<std::chrono::duration<double, typename std::chrono::minutes::period>>(duration).count() << " mins. ("
                                                        << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << " secs.)\n";
}
