#include "message_dump.hpp"
#include "loggers.hpp"
#include "transcoder/type_name.hpp"
#include "md_prsr/nasdaq/itch_v5.0/io.hpp"
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/list.hpp>
#include <variant>

std::ptrdiff_t message_dumper::operator()(tc::byte_t const* begin, tc::byte_t const* end) {
    using header = std::uint16_t;
    while (sizeof(header) <= static_cast<std::size_t>(std::distance(begin, end))) {
        auto size = nasdaq::itch::v5_0::decode<header>(begin, end);
        if (size > std::distance(begin, end)) {
            begin -= sizeof(header);
            break;
        }
        [[maybe_unused]] auto const message_end = begin + size;
        auto const decoded = nasdaq::itch::v5_0::decode<tc::as_flyweight<nasdaq::itch::v5_0::messages>>(begin, end);
        std::visit([this](auto const& message) {
           BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << ++count_ << ':' << message;
        }, decoded);
        assert(message_end == begin);
    }
    return std::distance(begin, end);
}

void message_dumper::done(std::chrono::system_clock::duration duration) {
    BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << "--\n" << count_ << " messages has been read in "
        << std::chrono::duration_cast<std::chrono::duration<double, typename std::chrono::minutes::period>>(duration).count() << " mins. ("
        << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << " secs.)\n";
}
