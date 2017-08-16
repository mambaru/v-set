//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_NODE_FOR_INSERT_HPP
#define VSET_VTREE_ASPECT_AD_NODE_FOR_INSERT_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <stdexcept>

namespace vset{ namespace vtree{

struct ad_node_for_insert
{
  template<typename T, typename V>
  bool less(T& t, const V& first, const V& second) const
  {
    return t.get_aspect().template get<_key_compare_>()(first, second);
  }

  template<typename T, typename V>
  bool less_equal(T& t, const V& first, const V& second) const
  {
    return less(t, first, second) || !less(t, second, first);
  }

  template<typename T, typename Itr, typename V>
  Itr operator()(T& t, Itr itr1, Itr itr2, const V& value ) const
  {
    if ( less_equal(t, value, itr1->first.first) )
    {
      return itr1;
    }

    if ( less_equal(t, itr2->first.first, value  ) )
    {
      return itr2;
    }

    if ( less_equal(t, itr1->first.second, value)
      && less_equal(t, value, itr2->first.first) )
    {
      // Можно вставить в любой, выбираем меньший
      return itr1->second->size() < itr2->second->size() ? itr1 : itr2;
    }

    if ( less_equal(t, itr1->first.first, value )
      && less_equal(t, value, itr1->first.second ) )
    {
      return itr1;
    }

    if ( less_equal(t, itr2->first.first, value)
      && less_equal(t, value, itr2->first.second ) )
    {
      return itr2;
    }

    throw std::logic_error("ad_node_for_insert");
  }
  
};

  
}}

#endif
