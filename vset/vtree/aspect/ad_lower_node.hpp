//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_LOWER_NODE_HPP
#define VSET_VTREE_ASPECT_LOWER_NODE_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <vset/nullptr.hpp>

namespace vset{ namespace vtree{

struct ad_lower_node
{
  template<typename T>
  typename T::container_type::iterator
  operator()(T& t, const typename T::key_type& value)
  {
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator iterator;

    container_type& container = t.get_container();
    iterator itr = container.lower_bound( std::make_pair(value, value ) );

    if ( itr == container.end() && !container.empty() )
    {
      itr = (++container.rbegin()).base();
    }

    // value < itr->first.first
    if ( itr!=container.end()
      && itr!=container.begin()
      && t.get_aspect().template get<_key_compare_>()(value, itr->first.first) )
    {
      --itr;
    }

    if ( itr != container.end())
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
    typedef typename container_type::const_iterator const_iterator;

    const container_type& container = t.get_container();
    const_iterator itr = container.lower_bound( std::make_pair(value, value ) );

    if ( itr == container.cend() && !container.empty() )
    {
      itr = (++container.crbegin()).base();
    }

    if ( itr != container.cend()
      && itr != container.cbegin()
      && t.get_aspect().template get<_value_compare_>()(value, itr->first.first) )
    {
      --itr;
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
