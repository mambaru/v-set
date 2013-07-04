//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_LOWER_BOUND_HPP
#define VSET_VTREE_ASPECT_AD_LOWER_BOUND_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <algorithm>

namespace vset{ namespace vtree{

struct ad_lower_bound
{
  template<typename T>
  typename T::iterator
  operator()(T& t, const typename T::value_type& value)
  {
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator container_iterator;

    typedef typename T::iterator iterator;
    typedef typename T::allocator_type          allocator_type;
    typedef typename allocator_type::value_type array_type;
    typedef typename array_type::iterator       array_iterator;
    typedef typename allocator_type::pointer    array_pointer;
    
    container_type& container = t.get_container();

    if ( container.empty() )
      return t.end();

    container_iterator cont_itr = t.get_aspect().template get<_lower_node_>()(t, value);

    if ( cont_itr == container.end() )
      return t.end();
    /*
    array_iterator itr = cont_itr->second->begin();
    for (;itr!=cont_itr->second->end() && t.get_aspect().template get<_compare_>()(value, *itr); ++itr);
    */
      
    
    array_iterator itr = std::lower_bound(
      cont_itr->second->begin(),
      cont_itr->second->end(),
      value,
      t.get_aspect().template get<_compare_>()
    );
    

    if ( itr == cont_itr->second->end() )
      return ++iterator( cont_itr, cont_itr->second->size()-1 );
      /*return t.end();*/

    return iterator( cont_itr, std::distance(cont_itr->second->begin(), itr) );
  }
};

}}

#endif
