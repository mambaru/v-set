//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_ASPECT_TREE_HPP
#define VSET_VTREE_ASPECT_ASPECT_TREE_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <fas/type_list/type_list_n.hpp>
#include <fas/aop.hpp>
#include <map>

namespace vset{ namespace vtree{

struct ad_multimap
{
  template<typename K, typename V, typename C>
  struct apply
  {
    typedef std::multimap<K, V, C> type;
  };
};

struct ad_get_key
{
  template<typename T, typename V>
  const typename T::key_type& operator()(T& t, const typename T::value_type& value) const
  {
    return value;
  }
};

template<typename V, typename Compare >
struct aspect_tree: fas::aspect< typename fas::type_list_n<
  fas::type< _value_type_, V >,
  fas::advice< _get_key_, ad_get_key>,
  fas::alias< _key_type_, _value_type_ >,
  fas::value< _compare_, Compare >,
  fas::alias< _key_compare_, _compare_ >,
  fas::alias< _value_compare_, _compare_ >,
  fas::advice< _container_, ad_multimap >
>::type> {};


}}

#endif
