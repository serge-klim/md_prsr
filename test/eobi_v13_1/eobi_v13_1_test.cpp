#define BOOST_TEST_MODULE eobi_v13_1_tests
#include "cmp.hpp"
#include "md_prsr/eobi/v13_1/messages.hpp"
#include "md_prsr/eobi/v13_1/describe.hpp"
#include "transcoder/transcoder.hpp"
#include "transcoder/type_name.hpp"
#include "transcoder/byte_order.hpp"
#include <boost/mp11/algorithm.hpp>
#include <boost/test/unit_test.hpp>
#include <numeric>
#include <type_traits>


namespace tc { 
template <std::size_t N>
constexpr bool operator == (tc::padding<N>, tc::padding<N>) noexcept { return true; }

} // namespace tc

BOOST_AUTO_TEST_SUITE(eobi_v13_1_test_sute)

BOOST_AUTO_TEST_CASE_TEMPLATE(encode_decode_message_test, message, eobi::v13_1::messages) {
   auto data = std::array<tc::byte_t, 1500>{};
   std::iota(begin(data), end(data), 11);
   auto id = tc::type_id<message>{}();
   auto begin = data.data();
   using deduced_options = tc::effective_options<message>;
   tc::wrapped::encode<deduced_options>(id, begin + sizeof(eobi::v13_1::MessageHeaderComp::bodyLen), begin + data.size());
   auto decoded_variant = tc::decode<eobi::v13_1::messages>(begin, begin + data.size());
   std::visit([&data](auto const& msg) {
      if constexpr (!std::is_same_v<decltype(msg), message const&>)
         BOOST_CHECK_MESSAGE(false, "wrong type has been decoded: " << tc::type_name<decltype(msg)>{}() << " instead of " << tc::type_name<message>{}());
      auto out = tc::encode(msg);
      BOOST_REQUIRE_LE(out.size(), data.size());
      // auto begin = data.data();
      // BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(out), cend(out), begin, begin + out.size()); //won't work with tc::padding
      auto begin_out = out.data();
      auto decoded1 = tc::decode<message>(begin_out, begin_out + out.size());
      if constexpr (std::is_same_v<decltype(decoded1) const&, decltype(msg)>)
         BOOST_CHECK(test::cmp(decoded1, msg));
      else
         BOOST_CHECK_MESSAGE(false, "wrong type has been decoded: " << tc::type_name<decltype(msg)>{}() << " instead of " << tc::type_name<message>{}());
      BOOST_CHECK_EQUAL(std::distance(out.data(), begin_out), out.size());
   },
              decoded_variant);
}

BOOST_AUTO_TEST_SUITE_END()
