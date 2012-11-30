//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_MULTIMAP_INSERT_HPP
#define VSET_VTREE_ASPECT_AD_MULTIMAP_INSERT_HPP

#include <vset/vtree/aspect/tags.hpp>

namespace vset{ namespace vtree{

struct ad_multimap_insert
{
  template<typename T, typename Itr, typename K, typename V>
  Itr operator()(T& t, Itr itr, const K& key, const V& value )
  {
    return t.get_container().insert( itr, std::make_pair(key, value) );
  }

  template<typename T, typename K, typename V>
  typename T::container_type::iterator
  operator()(T& t, const K& key, const V& value )
  {
    return t.get_container().insert( std::make_pair(key, value) );
  }
};

  
}}

#endif
