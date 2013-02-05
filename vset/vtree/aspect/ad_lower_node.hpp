//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_LOWER_NODE_HPP
#define VSET_VTREE_ASPECT_LOWER_NODE_HPP

#include <vset/vtree/aspect/tags.hpp>

namespace vset{ namespace vtree{

struct ad_lower_node
{
  template<typename T>
  typename T::container_type::iterator
  operator()(T& t, const typename T::value_type& value)
  {
    typedef typename T::value_type value_type;
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator iterator;

    container_type& container = t.get_container();
    iterator itr = container.lower_bound( std::make_pair(value, value/*_type()*/ ) );

    if ( itr==container.end() && !container.empty() )
      itr = --container.end()/*(++container.rbegin()).base()*/;

    if ( itr!=container.end() && itr!=container.begin()
         && t.get_aspect().template get<_compare_>()(value, itr->first.first) )
    {
      --itr;
    }

    return  itr;
  }
};

}}

#endif
