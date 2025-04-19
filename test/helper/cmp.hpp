#pragma once
//#include <boost/describe/operators.hpp>
#include <boost/describe.hpp>
#include <concepts>
#include <cstring>

namespace test { inline namespace v1 {

template <typename T>
   requires std::equality_comparable<T>
bool cmp(T const& l, T const& r) {
   return l == r;
}

template <typename T>
   requires(!std::equality_comparable<T> && !boost::describe::has_describe_members<T>::value)
bool cmp(T const& l, T const& r) {
   return std::memcmp(&l, &r, sizeof(T)) == 0;
}

template <typename T>
   requires(!std::equality_comparable<T> && boost::describe::has_describe_members<T>::value)
bool cmp(T const& l, T const& r) {
   auto res = true;
   boost::mp11::mp_for_each<boost::describe::describe_members<T, boost::describe::mod_public>>([&](auto D) {
      res = res && test::cmp(l.*D.pointer, r.*D.pointer);
   });
   return res;
}

}} // namespace test::v1