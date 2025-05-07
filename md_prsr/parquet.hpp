#pragma once
#include "pqx/parquet.hpp"
#include "transcoder/tagged_t.hpp"

template <typename T, typename Tag, typename Options>
struct pqx::node_builder<tc::tagged_t<T, Tag>, Options, std::true_type> {
   parquet::schema::NodePtr operator()(std::string const& name) const {
      return node_builder<T, Options>{}(name);
   }
};


template <typename T, typename Tag>
struct pqx::column_writer<tc::tagged_t<T, Tag>, std::true_type> {
   void operator()(row_group_writer& row_group, T value) const {
      column_writer<T>{}(row_group, static_cast<T&>(value));
   }
   void operator()(row_group_writer& row_group, std::nullptr_t) const {
      column_writer<T>{}(row_group, nullptr);
   }
};

