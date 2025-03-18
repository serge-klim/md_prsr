#pragma once
#include "messages.hpp"
#include "transcoder/io.hpp"
#include <iostream>

namespace tc { inline namespace v1 { namespace io {

template<>
struct formatter<nasdaq::itch::v5_0::timestamp_t, std::true_type> {
    std::ostream& operator()(std::ostream& out, nasdaq::itch::v5_0::timestamp_t const& v) const {
      out << v.value;
      return out;
   }
};

}}} // namespace tc::v1::io

namespace nasdaq::itch::v5_0 {

template <typename T>
std::ostream& operator<<(std::ostream& out, T const& value) {
   return tc::io::dump(out, value);
}

} // namespace nasdaq::itch::v5_0








