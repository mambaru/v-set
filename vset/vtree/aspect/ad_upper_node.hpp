//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_UPPER_NODE_HPP
#define VSET_VTREE_ASPECT_UPPER_NODE_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <vset/nullptr.hpp>

namespace vset{ namespace vtree{

struct ad_upper_node
{
  template<typename T>
  typename T::container_type::iterator
  operator()(T& t, const typename T::key_type& value)
  {
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator container_iterator;
    container_type& container = t.get_container();

    container_iterator itr = t.get_aspect().template get<_lower_node_>()(t, value);
    for ( ; itr!=container.end()
      && !t.get_aspect().template get<_compare_>()(value, itr->first.first)
      && !t.get_aspect().template get<_compare_>()(itr->first.first, value)
      ; ++itr )
    {
    }

    if ( itr != container.end() )
    {
      if ( itr != container.begin() && t.get_aspect().template get<_compare_>()(value, itr->first.first) )
      {
        --itr;
      }
    }
    else
    {
      if ( !container.empty() )
      {
        itr = (++container.rbegin()).base();
      }
    }

    if ( itr != container.end() )
    {
      VSET_NULLPTR_ACCERT(itr->second)
    }

    return  itr;
  }

  template<typename T>
  typename T::container_type::const_iterator
  operator()(const T& t, const typename T::key_type& value) const
  {
    typedef typename T::container_type container_type;
    typedef typename container_type::const_iterator const_container_iterator;
    const container_type& container = t.get_container();

    const_container_iterator itr = t.get_aspect().template get<_lower_node_>()(t, value);
    for ( ; itr != container.cend()
      && !t.get_aspect().template get<_compare_>()(value, itr->first.first)
      && !t.get_aspect().template get<_compare_>()(itr->first.first, value)
      ; ++itr )
    {
    }

    if ( itr != container.cend() )
    {
      if ( itr != container.cbegin() && t.get_aspect().template get<_compare_>()(value, itr->first.first) )
      {
        --itr;
      }
    }
    else
    {
      if ( !container.empty() )
      {
        itr = (++container.crbegin()).base();
      }
    }

    if ( itr != container.end() )
    {
      VSET_NULLPTR_ACCERT(itr->second)
    }

    return itr;
  }
};

}}

#endif
