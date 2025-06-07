#pragma once
#include "md_prsr/nasdaq/itch_v5.0/transcoder.hpp"
#include "md_prsr/nasdaq/moldudp64_v1.0/messages.hpp"
#include "transcoder/flyweight.hpp"
#include <boost/mp11/list.hpp>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <chrono>

class instrument_stats {
    using stock_locate_t = decltype(nasdaq::itch::v5_0::add_order::stock_locate);
 public:
   // constexpr auto const& messages() const noexcept { return messages_; }
   std::ptrdiff_t operator()(nasdaq::moldudp64::v1_0::seqn_t seqn, tc::byte_t const* begin, tc::byte_t const* end);
   void done(std::chrono::system_clock::duration duration);
 private:
   template <typename Options>
   void process_message(nasdaq::moldudp64::v1_0::seqn_t seqn, tc::flyweight<nasdaq::itch::v5_0::system_event, Options> const& fw);

   template <typename Options>
   void process_message(nasdaq::moldudp64::v1_0::seqn_t seqn, tc::flyweight<nasdaq::itch::v5_0::stock_directory, Options> const& fw);


   template <typename T, typename Options>
   void process_message(nasdaq::moldudp64::v1_0::seqn_t seqn, tc::flyweight<T, Options> const& fw);

private:
   using order_messages = boost::mp11::mp_list<nasdaq::itch::v5_0::add_order, nasdaq::itch::v5_0::add_order_mpid, nasdaq::itch::v5_0::order_cancel, nasdaq::itch::v5_0::order_delete, nasdaq::itch::v5_0::order_replace>;
   using other_messages = boost::mp11::mp_list<nasdaq::itch::v5_0::order_executed, nasdaq::itch::v5_0::order_executed_with_price, 
                                                nasdaq::itch::v5_0::trade,nasdaq::itch::v5_0::cross_trade, nasdaq::itch::v5_0::broken_trade_message>;
   using messages_set_t = boost::mp11::mp_append<order_messages, other_messages>;
   using messages_stats = std::array<std::size_t, boost::mp11::mp_size<messages_set_t>::value>;
   std::unordered_map<stock_locate_t, messages_stats> stats_;

   struct stock_directory_hash {
      using is_transparent = void;
      auto operator()(decltype(nasdaq::itch::v5_0::stock_directory::stock_locate) value) const noexcept {
         return std::hash<decltype(nasdaq::itch::v5_0::stock_directory::stock_locate)>{}(value);
      }
      auto operator()(nasdaq::itch::v5_0::stock_directory const& value) const noexcept {
         return operator()(value.stock_locate);
      }
   };
   struct stock_directory_eq {
      using is_transparent = void;
      auto operator()(nasdaq::itch::v5_0::stock_directory const& l, nasdaq::itch::v5_0::stock_directory const& r) const noexcept {
         return std::equal_to<decltype(nasdaq::itch::v5_0::stock_directory::stock_locate)>{}(l.stock_locate, r.stock_locate);
      }
      auto operator()(decltype(nasdaq::itch::v5_0::stock_directory::stock_locate) l, nasdaq::itch::v5_0::stock_directory const& r) const noexcept {
         return std::equal_to<decltype(nasdaq::itch::v5_0::stock_directory::stock_locate)>{}(l, r.stock_locate);
      }
      auto operator()(nasdaq::itch::v5_0::stock_directory const& l, decltype(nasdaq::itch::v5_0::stock_directory::stock_locate) r) const noexcept {
         return std::equal_to<decltype(nasdaq::itch::v5_0::stock_directory::stock_locate)>{}(l.stock_locate, r);
      }
   };
   std::unordered_set<nasdaq::itch::v5_0::stock_directory, stock_directory_hash, stock_directory_eq> stock_directory_;
   std::vector<std::pair<nasdaq::moldudp64::v1_0::seqn_t, nasdaq::itch::v5_0::system_event>> system_events_;
   std::size_t stock_dir_mods_ = 0;
};

template <typename T, typename Options>
void instrument_stats::process_message(nasdaq::moldudp64::v1_0::seqn_t /*seqn*/, tc::flyweight<T, Options> const& fw) {
   if constexpr (constexpr auto ix = boost::mp11::mp_find<messages_set_t, T>::value; ix < std::tuple_size<messages_stats>::value) {
      ++stats_[tc::get<&T::stock_locate>(fw)][ix];
   }
}

template <typename Options>
void instrument_stats::process_message(nasdaq::moldudp64::v1_0::seqn_t seqn, tc::flyweight<nasdaq::itch::v5_0::system_event, Options> const& fw) {
   system_events_.emplace_back(std::make_pair(seqn, fw.value()));
}

template <typename Options>
void instrument_stats::process_message(nasdaq::moldudp64::v1_0::seqn_t /*seqn*/, tc::flyweight<nasdaq::itch::v5_0::stock_directory, Options> const& fw) {
   auto [i,ok] = stock_directory_.emplace(fw.value());
   if (!ok) [[unlikely]] {
      ++stock_dir_mods_;
      stock_directory_.erase(i);
      stock_directory_.emplace(fw.value());
   }
}
