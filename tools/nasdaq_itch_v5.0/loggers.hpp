#pragma once
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/program_options.hpp>
//#include <boost/log/expressions.hpp>
#include <string>
#include <cstdint>

using logger = boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level, std::string>;
BOOST_LOG_GLOBAL_LOGGER(log, logger)

using stats_logger = boost::log::sources::severity_channel_logger<boost::log::trivial::severity_level, std::string>;
BOOST_LOG_GLOBAL_LOGGER(stats_log, stats_logger)

BOOST_LOG_ATTRIBUTE_KEYWORD(queue, "queue", std::uint8_t)

//BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
void configure_default_logger(boost::program_options::variables_map const& options);

constexpr auto default_latency_n_packets = std::size_t{ 16 };
constexpr auto default_throughput_n_packets = std::size_t{ 10000 };
