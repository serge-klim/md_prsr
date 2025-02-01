#pragma once
#include "timestamp.hpp"
#include <variant>
#include <array>
#include <cstdint>
#include <chrono>

//https://www.nasdaqtrader.com/content/technicalsupport/specifications/dataproducts/NQTVITCHSpecification.pdf
namespace nasdaq::itch::v5_0{

using stock_t = std::array<char,8>;
//using timestamp_t = std::array<std::uint8_t, 6>;
//using timestamp_t = std::chrono::duration<std::uint64_t, std::nano>;
//BOOST_STRONG_TYPEDEF(std::uint64_t, timestamp_t);
using price_t = std::uint32_t;    

//1.1 System Event Message
struct system_event
{
    char message_type = 'S';
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    timestamp_t timestamp;
    char event_code;
};
   
//1.2.1 Stock Directory
struct stock_directory {
    char message_type = 'R';
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    timestamp_t timestamp;
    stock_t stock;
    char market_category; // 'Q', 'G', 'S', 'N', 'A', 'P', 'Z', 'V'
    char financial_status_indicator; // 'D', 'E', 'Q', 'S', 'G', 'N'
    std::uint32_t round_lot_size;
    char round_lots_only; // 'Y' or 'N'
    char issue_classification;
    std::array<char,2> issue_sub_type;
    char authenticity; // 'P' or 'T'
    char short_sale_threshold_indicator; // 'Y' or 'N'
    char ipo_flag; // 'Y' or 'N'
    char luld_reference_price_tier; // '1', '2', '3'
    char etp_flag; // 'Y' or 'N'
    std::uint32_t etp_leverage_factor;
    char inverse_indicator; // 'Y' or 'N'
};
   

//1.2.2 Stock Trading Action        
struct stock_trading_action {
    char message_type = 'H';
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    timestamp_t timestamp;
    stock_t stock;
    char trading_state; // 'H', 'P', 'Q', 'T'
    char reserved;
    std::array<char,4> reason;
};
    
   
//1.2.3 Reg SHO Short Sale Price Test Restricted Indicator    
struct reg_sho
{
    char message_type = 'Y';
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    timestamp_t timestamp;  
    stock_t stock;
    char reg_sho_action;
};
    
//1.2.4 Market Participant Position
struct market_participant_position
{
    char message_type = 'L';
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    timestamp_t timestamp; 
    std::array<char,4> mpid;
    stock_t stock;
    char primary_market_maker;
    char market_maker_mode;
    char market_participant_state;
};

//1.2.5.1 MWCB Decline Level Message    
struct mwcb_decline_level
{
    char message_type = 'V';
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    timestamp_t timestamp; 
    std::uint64_t level1;
    std::uint64_t level2;
    std::uint64_t level3; 
};
    
//1.2.5.2 MWCB Status Message
struct mwcb_status
{
    char message_type = 'W';
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    timestamp_t timestamp;
    char breached_level; // Denotes the MWCB Level that was breached. '1' = Level 1 '2' = Level 2 '3' = Level 3
};
    
//1.2.6 Quoting Period Update    
struct quoting_period_update
{
    char message_type = 'K';
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    timestamp_t timestamp;
    stock_t stock;
    std::uint32_t ipo_quotation_release_time;
    char ipo_release_qualifier; // "A" = Anticipated Quotation Release Time / "C" = IPO Release Canceled/Postponed
    price_t ipo_price;
};

//1.2.7 Limit Up - Limit Down (LULD) Auction Collar
struct luld
{
    char message_type = 'J';
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    timestamp_t timestamp;
    stock_t stock;
    price_t auction_collar_reference_price;
    price_t upper_auction_collar_price;
    price_t lower_auction_collar_price;
    price_t auction_collar_extension;    
};

//1.2.8 Operational Halt    
struct operational_halt
{
    char message_type = 'h';
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    timestamp_t timestamp;
    stock_t stock;
    char market_code; //'Q': Nasdaq ; 'B' : BX ; 'X' : PSX
    char operational_halt_action; //'H': Operationally Halted on the identified Market ; 'T': Operational Halt has been lifted and Trading resume
};
     
    
// 1.3.1 Add Order - No MPID Attribution
 struct add_order {
     char message_type = 'A';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t order_reference_number;
     char buy_sell_indicator; // 'B' or 'S'
     std::uint32_t shares;
     stock_t stock;
     price_t price;
 };

 //1.3.2 Add Order with MPID Attribution
 struct add_order_mpid {
     char message_type = 'F';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t order_reference_number;
     char buy_sell_indicator; // 'B' or 'S'
     std::uint32_t shares;
     stock_t stock;
     price_t price;
     std::array<char, 4> attribution;
 };

  //1.4.1 Order Executed Message
 struct order_executed {
     char message_type = 'E';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t order_reference_number;
     std::uint32_t executed_shares;
     std::uint64_t match_number;
 };

 // 1.4.2 Order Executed With Price Message
 struct order_executed_with_price {
     char message_type = 'C';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t order_reference_number;
     std::uint32_t executed_shares;
     std::uint64_t match_number;
     char printable; // 'N' = Non - Printable 'Y' = Printable
     price_t execution_price;
 };

// 1.4.3 Order Cancel Message
 struct order_cancel {
     char message_type = 'X';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t order_reference_number;
     std::uint32_t canceled_shares;
 };

// 1.4.4 Order Delete Message
 struct order_delete {
     char message_type = 'D';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t order_reference_number;
 };

// 1.4.5. Order Replace Message
 struct order_replace {
     char message_type = 'U';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t original_order_reference_number;
     std::uint64_t new_order_reference_number;
     std::uint32_t shares;
     price_t price;
 };

 
// 1.5.1 Trade Message (Non-Cross)
 struct trade {
     char message_type = 'P';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t order_reference_number;
     char buy_sell_indicator; // 'B' or 'S'
     std::uint32_t shares;
     stock_t stock;
     price_t price;
     uint64_t match_number;
 };

 // 1.5.2 Cross Trade Message
 struct cross_trade {
     char message_type = 'Q';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t shares;
     stock_t stock;
     std::uint32_t cross_price;
     std::uint64_t match_number;
     char cross_type; // 'O'  'C' 'H'
 };

// 1.5.3 Broken Trade / Order Execution Message
 struct broken_trade_message {
     char message_type = 'B';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t match_number;
 };

// 1.6 Net Order Imbalance Indicator (NOII) Message
 struct noii {
     char message_type = 'I';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     std::uint64_t paired_shares;
     std::uint64_t imbalance_shares;
     char imbalance_direction; // 'B', 'S', 'N', 'O'
     stock_t stock;
     price_t far_price;
     price_t near_price;
     price_t current_reference_price;
     char cross_type; // 'O', 'C', 'H', 'I'
     char price_variation_indicator; // 'L', '1', '2', '3', '4'
 };

// Retail Price Improvement Indicator (RPII) Message
 struct rpii {
     char message_type = 'N';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     stock_t stock;
     char interest_flag; // 'B', 'S', 'A'
 };

 
 //1.8 Direct Listing with Capital Raise Price Discovery Message
 struct direct_listing_with_capital_raise_price_discovery {
     char message_type = 'O';
     std::uint16_t stock_locate;
     std::uint16_t tracking_number;
     timestamp_t timestamp;
     stock_t stock;
     char open_eligibility_status;
     price_t minimum_allowable_price;
     price_t maximum_allowable_price;
     price_t near_execution_price;
     std::uint64_t near_execution_time;
     price_t lower_price_range_collar;
     price_t upper_price_range_collar;
 };

using messages = std::variant<
    system_event,
    stock_directory,
    stock_trading_action,
    reg_sho,
    market_participant_position,
    mwcb_decline_level,
    mwcb_status,
    quoting_period_update,
    luld,
    operational_halt,
    add_order,
    add_order_mpid,
    order_executed,
    order_executed_with_price,
    order_cancel,
    order_delete,
    order_replace,  
    trade,
    cross_trade,
    broken_trade_message,
    noii,
    rpii,
    direct_listing_with_capital_raise_price_discovery
 >;

} // namespace nasdaq::itch::v5_0    

