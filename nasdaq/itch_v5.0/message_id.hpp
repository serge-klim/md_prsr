#pragma once
#include "messages.hpp"
#include "transcoder/traits.hpp"

template<> struct tc::type_id<nasdaq::itch::v5_0::system_event> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::system_event{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::stock_directory> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::stock_directory{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::stock_trading_action> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::stock_trading_action{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::reg_sho> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::reg_sho{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::market_participant_position> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::market_participant_position{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::mwcb_decline_level> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::mwcb_decline_level{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::mwcb_status> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::mwcb_status{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::quoting_period_update> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::quoting_period_update{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::luld> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::luld{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::operational_halt> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::operational_halt{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::add_order> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::add_order{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::add_order_mpid> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::add_order_mpid{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::order_executed> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::order_executed{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::order_executed_with_price> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::order_executed_with_price{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::order_cancel> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::order_cancel{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::order_delete> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::order_delete{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::order_replace> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::order_replace{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::trade> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::trade{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::cross_trade> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::cross_trade{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::broken_trade_message> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::broken_trade_message{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::noii> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::noii{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::rpii> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::rpii{}.message_type; } };
template<> struct tc::type_id<nasdaq::itch::v5_0::direct_listing_with_capital_raise_price_discovery> { constexpr char operator()() const noexcept { return nasdaq::itch::v5_0::direct_listing_with_capital_raise_price_discovery{}.message_type; } };

