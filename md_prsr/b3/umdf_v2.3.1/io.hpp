#pragma once
#include "describe.hpp"
#include "md_prsr/io.hpp"
#include "md_prsr/sbe/io.hpp"
#include "io.hpp"
#include <boost/describe.hpp>
#include <iostream>
#include <type_traits>

namespace b3::umdf::v2_2_0 {

template <typename T>
auto operator<<(std::ostream& out, T const& value) -> std::enable_if_t<boost::describe::has_describe_members<T>::value, std::ostream&> {
   return tc::io::dump(out, value);
}

} // namespace b3::umdf::v2_2_0








