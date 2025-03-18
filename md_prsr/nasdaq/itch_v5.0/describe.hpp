#pragma once
#include "messages.hpp"
#include <boost/describe.hpp>

namespace nasdaq::itch::v5_0{

//1.1 System Event Message
BOOST_DESCRIBE_STRUCT(system_event, (),
(
    message_type,
    stock_locate,
    tracking_number,
    timestamp,
    event_code
))
   
//1.2.1 Stock Directory
BOOST_DESCRIBE_STRUCT(stock_directory, (),
(
    message_type,
    stock_locate,
    tracking_number,
    timestamp,
    stock,
    market_category, 
    financial_status_indicator,
    round_lot_size,
    round_lots_only,
    issue_classification,
    issue_sub_type,
    authenticity,
    short_sale_threshold_indicator,
    ipo_flag,
    luld_reference_price_tier,
    etp_flag,
    etp_leverage_factor,
    inverse_indicator
))
   

////1.2.2 Stock Trading Action        
BOOST_DESCRIBE_STRUCT(stock_trading_action, (),
(
    message_type,
    stock_locate,
    tracking_number,
    timestamp,
    stock,
    trading_state, // 'H', 'P', 'Q', 'T
    reserved,
    reason
))
   
////1.2.3 Reg SHO Short Sale Price Test Restricted Indicator    
BOOST_DESCRIBE_STRUCT(reg_sho, (),
(
    message_type,
    stock_locate,
    tracking_number,
    timestamp,
    stock,
    reg_sho_action
))

////1.2.4 Market Participant Position
BOOST_DESCRIBE_STRUCT(market_participant_position, (),
(
    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    mpid,
    stock,
    primary_market_maker,
    market_maker_mode,
    market_participant_state
))
 
////1.2.5.1 MWCB Decline Level Message    
BOOST_DESCRIBE_STRUCT(mwcb_decline_level, (),
(
    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    level1,
    level2,
    level3
))

////1.2.5.2 MWCB Status Message
BOOST_DESCRIBE_STRUCT(mwcb_status, (),
(
    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    breached_level
))

////1.2.6 Quoting Period Update    
BOOST_DESCRIBE_STRUCT(quoting_period_update, (),
(
    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    stock,
    ipo_quotation_release_time,
    ipo_release_qualifier, // "A" = Anticipated Quotation Release Time / "C" = IPO Release Canceled/Postponed
    ipo_price
))

////1.2.7 Limit Up - Limit Down (LULD) Auction Collar
 BOOST_DESCRIBE_STRUCT(luld, (),
 (
    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    stock,
    auction_collar_reference_price,
    upper_auction_collar_price,
    lower_auction_collar_price,
    auction_collar_extension
))

////1.2.8 Operational Halt    
BOOST_DESCRIBE_STRUCT(operational_halt, (),
(
    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    stock,
    market_code,
    operational_halt_action
))
 
//// 1.3.1 Add Order - No MPID Attribution
BOOST_DESCRIBE_STRUCT(add_order, (),
(    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    order_reference_number,
    buy_sell_indicator, // 'B' or 'S'
    shares,
    stock,
    price
))
 
// //1.3.2 Add Order with MPID Attribution
BOOST_DESCRIBE_STRUCT(add_order_mpid, (),
(    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    order_reference_number,
    buy_sell_indicator, // 'B' or 'S'
    shares,
    stock,
    price,
    attribution
))

 //1.4.1 Order Executed Message
BOOST_DESCRIBE_STRUCT(order_executed, (),
(    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    order_reference_number,
    executed_shares,
    match_number
))

// 1.4.2 Order Executed With Price Message
BOOST_DESCRIBE_STRUCT(order_executed_with_price, (),
(    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    order_reference_number,
    executed_shares,
    match_number,
    printable, // 'N' = Non - Printable 'Y' = Printable
    execution_price
))

//// 1.4.3 Order Cancel Message
BOOST_DESCRIBE_STRUCT(order_cancel, (),
(   message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    order_reference_number,
    canceled_shares
))
 
//// 1.4.4 Order Delete Message
BOOST_DESCRIBE_STRUCT(order_delete, (),
(   message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    order_reference_number
))

//// 1.4.5. Order Replace Message
BOOST_DESCRIBE_STRUCT(order_replace, (),
(   message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    original_order_reference_number,
    new_order_reference_number,
    shares,
    price
))
 
//// 1.5.1 Trade Message (Non-Cross)
BOOST_DESCRIBE_STRUCT(trade, (),
(   message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    order_reference_number,
    buy_sell_indicator, // 'B' or 'S'
    shares,
    stock,
    price,
    match_number
))
 
// // 1.5.2 Cross Trade Message
BOOST_DESCRIBE_STRUCT(cross_trade, (),
(    
    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    shares,
    stock,
    cross_price,
    match_number,
    cross_type
))


//// 1.5.3 Broken Trade / Order Execution Message
BOOST_DESCRIBE_STRUCT(broken_trade_message, (),
(    
    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    match_number
))

//// 1.6 Net Order Imbalance Indicator (NOII) Message
BOOST_DESCRIBE_STRUCT(noii, (),
(    
    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    paired_shares,
    imbalance_shares,
    imbalance_direction, // 'B', 'S', 'N', 'O'
    stock,
    far_price,
    near_price,
    current_reference_price,
    cross_type, // 'O', 'C', 'H', 'I'
    price_variation_indicator
))

//// Retail Price Improvement Indicator (RPII) Message
BOOST_DESCRIBE_STRUCT(rpii, (),
(    
    message_type,
    stock_locate,
    tracking_number,
    timestamp, 
    stock,
    interest_flag
))
 
//1.8 Direct Listing with Capital Raise Price Discovery Message
BOOST_DESCRIBE_STRUCT(direct_listing_with_capital_raise_price_discovery, (),
(    
    message_type,
    stock_locate,
    tracking_number,
    timestamp,
    stock,
    open_eligibility_status,
    minimum_allowable_price,
    maximum_allowable_price,
    near_execution_price,
    near_execution_time,
    lower_price_range_collar,
    upper_price_range_collar
))


} // namespace nasdaq::itch::v5_0

