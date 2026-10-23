#pragma once
#include "transcoder/io.hpp"
#include "optional.hpp"
#include <optional>

namespace tc { inline namespace v1 { namespace io {

template <typename T, T Null>
struct formatter<md_prsr::optional<T, Null>, std::true_type> {
   std::ostream& operator()(std::ostream& out, md_prsr::optional<T, Null> const& val) const {
      return formatter<std::optional<T>>{}(out, val);
   }
};

}}} // namespace tc::v1::io
