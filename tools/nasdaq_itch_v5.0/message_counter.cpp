#include "message_counter.hpp"
#include "loggers.hpp"
//#include "transcoder/type_name.hpp"
//#include "md_prsr/nasdaq/itch_v5.0/io.hpp"
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/list.hpp>
#include <variant>

std::ptrdiff_t message_counter::operator()(tc::byte_t const* begin, tc::byte_t const* end) {
    using header = std::uint16_t;
    while (sizeof(header) <= static_cast<std::size_t>(std::distance(begin, end))) {
        auto size = nasdaq::itch::v5_0::decode<header>(begin, end);
        if (size > std::distance(begin, end)) {
            begin -= sizeof(header);
            break;
        }
        [[maybe_unused]] auto const message_end = begin + size;
        tc::decode<tc::as_flyweight<nasdaq::itch::v5_0::messages>>(begin, end, [this](auto const& message){
           if constexpr (std::is_assignable_v<nasdaq::itch::v5_0::messages, decltype(message)>) 
                  ++messages_[message.type_id()];
            //BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << message;
            });
        assert(message_end == begin);
    }
    return std::distance(begin, end);
}

template<typename ...T>
auto make_type_name(std::type_identity<std::variant<T...>>) {
    using id_type = decltype(tc::type_id < boost::mp11::mp_first<boost::mp11::mp_list<T...>>>{}());
    return std::unordered_map<id_type, std::string>{ {tc::type_id<T>{}(), tc::type_name<T>{}()}..., };
}

void message_counter::done(std::chrono::system_clock::duration duration) {
    BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << messages_.size() << " from " << boost::mp11::mp_size<nasdaq::itch::v5_0::messages>::value << " messages types has been read";

    auto id_names = make_type_name(std::type_identity<nasdaq::itch::v5_0::messages>{});
    auto total = std::size_t{ 0 };
    for (auto [id, n] : messages_) {
        auto i = id_names.find(id);
        if (i == cend(id_names)) {
            BOOST_LOG_SEV(log::get(), boost::log::trivial::error) << "can't map id '" << id << "' to key.";
            continue;
        }
        BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << id << " (" << i->second << ") : " << n;
        total += n;
        id_names.erase(i);
    }
    BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << "--\n" << total << " messages in total has been read in "
        << std::chrono::duration_cast<std::chrono::duration<double, typename std::chrono::minutes::period>>(duration).count() << " mins. ("
        << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << " secs.)\n";

    BOOST_LOG_SEV(log::get(), boost::log::trivial::warning) << "following messages are missing :";
    for (auto const& [id, name] : id_names) {
        BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << id << ' ' << name;
    }
}
