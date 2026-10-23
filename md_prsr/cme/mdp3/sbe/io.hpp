#pragma once
#include "transcoder/io.hpp"
#include "sbe.hpp"

namespace tc { inline namespace v1 { namespace io {

template <typename T, typename GroupSize>
struct formatter<sbe::group<T, GroupSize>, std::true_type> {
   std::ostream& operator()(std::ostream& out, sbe::group<T, GroupSize> const& val) const {
      out << type_name<T>{}() << "{";
      for (auto const& i:val.data)
         out << "\n\t" << i;
      out << "\n}\n";
      return out;
   }
};

}}} // namespace tc::v1::io
