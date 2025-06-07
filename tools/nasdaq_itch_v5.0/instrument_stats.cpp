#include "instrument_stats.hpp"
#include "loggers.hpp"
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include "md_prsr/nasdaq/itch_v5.0/io.hpp"
#include "transcoder/type_name.hpp"
#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/list.hpp>
#include <map>
#include <string_view>
#include <algorithm>
#include <functional>
#include <ranges>
#include <variant>
#include <cassert>

std::ptrdiff_t instrument_stats::operator()(nasdaq::moldudp64::v1_0::seqn_t seqn, tc::byte_t const* begin, tc::byte_t const* end) {
   tc::decode<tc::as_flyweight<nasdaq::itch::v5_0::messages>>(begin, end, [this, seqn](auto const& fw) {
      auto error = handle_error(fw);
      if constexpr (!error.value) {
         process_message(seqn, fw);
         //using message_t = typename std::decay_t<decltype(fw)>::type;
         //if constexpr (constexpr auto ix = boost::mp11::mp_find<order_messages, message_t>::value; ix < std::tuple_size<messages_stats>::value) {
         //   ++stats_[tc::get<&message_t::stock_locate>(fw)][ix];
         //}
      }
   });
   assert(begin == end);
   return 0;
}

template <typename... T>
auto make_type_name(std::type_identity<std::variant<T...>>) {
   using id_type = decltype(tc::type_id<boost::mp11::mp_first<boost::mp11::mp_list<T...>>>{}());
   return std::unordered_map<id_type, std::string>{
       {tc::type_id<T>{}(), tc::type_name<T>{}()}...,
   };
}

void instrument_stats::done(std::chrono::system_clock::duration duration) {
   BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << stock_directory_.size() << " instruments deffenition has been received\n"
                                                        << stats_.size() << " instruments has been processed in in "
                                                        << std::chrono::duration_cast<std::chrono::duration<double, typename std::chrono::minutes::period>>(duration).count() << " mins. ("
                                                        << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << " secs.)"
                                                                                                                                 "\n\tsummarizing...";
   if (stock_dir_mods_) {
      BOOST_LOG_SEV(log::get(), boost::log::trivial::warning) << "stock_directory overriden : " << stock_dir_mods_;
   }

   auto desc = std::map<std::size_t, stock_locate_t, std::greater<std::size_t>>{}; // TODO: map is overkill here
   std::ranges::copy(stats_ | std::views::transform([](const auto& instrument_stats) {
                        return std::make_pair(std::ranges::fold_left(instrument_stats.second | std::views::take(boost::mp11::mp_size<order_messages>::value), 0, std::plus{}), instrument_stats.first);
                     }),
                     std::inserter(desc, std::end(desc)));

   std::ranges::for_each(desc | std::views::take(10), [this, &desc](auto v) {
      const auto& messages = stats_[v.second];
      auto stats = std::string{};
      boost::mp11::mp_for_each<boost::mp11::mp_iota<boost::mp11::mp_size<messages_set_t>>>([&messages, &stats](auto Ix) {
         stats += "\n\t";
         stats += tc::type_name<boost::mp11::mp_at_c<messages_set_t, Ix>>{}();
         stats += ' ';
         stats += std::to_string(messages[Ix]);
      });

      auto stock_dir = stock_directory_.find(v.second);
      auto stock = stock_dir == end(stock_directory_) ? std::string_view{"unknown"} : std::string_view{cbegin(stock_dir->stock), std::ranges::find(stock_dir->stock, ' ')};
      BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << stock << '[' << v.second << "] - " << v.first << stats;
   });

   for (auto const& [seqn, system_event] : system_events_)
      BOOST_LOG_SEV(log::get(), boost::log::trivial::info) << seqn << ':' << system_event;

}
