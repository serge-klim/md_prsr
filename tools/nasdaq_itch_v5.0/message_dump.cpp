#include "message_dump.hpp"
#include "loggers.hpp"
#include "transcoder/type_name.hpp"
#include "md_prsr/nasdaq/itch_v5.0/io.hpp"
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include <variant>

std::ptrdiff_t message_dumper::operator()(nasdaq::moldudp64::v1_0::seqn_t /*seqn*/, tc::byte_t const* begin, tc::byte_t const* end) {
    auto const decoded = nasdaq::itch::v5_0::decode<tc::as_flyweight<nasdaq::itch::v5_0::messages>>(begin, end);
    std::visit([this](auto const& message) {
        BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << ++count_ << ':' << message;
    }, decoded);
    assert(begin == end);
    return 0;
}

void message_dumper::done(std::chrono::system_clock::duration duration) {
    BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << "--\n" << count_ << " messages has been read in "
        << std::chrono::duration_cast<std::chrono::duration<double, typename std::chrono::minutes::period>>(duration).count() << " mins. ("
        << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << " secs.)\n";
}
