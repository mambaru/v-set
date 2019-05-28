//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_CREATE_NODE_HPP
#define VSET_VTREE_ASPECT_AD_CREATE_NODE_HPP

#include <fas/system/nullptr.hpp>
#include <vset/vtree/aspect/tags.hpp>
#include <string.h>
#include <stdlib.h>

namespace vset{ namespace vtree{

struct ad_create_node
{
  template<typename T>
  typename T::container_type::iterator
  operator()(T& t, const typename T::value_type& value)
  {
    typedef typename T::allocator_type allocator_type;
    typedef typename allocator_type::value_type array_type;
    typedef typename allocator_type::pointer pointer;

    pointer parr = t.get_allocator().allocate(1, fas_nullptr);
    t.get_allocator().construct(parr, array_type() );
    const typename T::key_type& key = t.get_aspect().template get<_get_key_>()(t, value);
    return t.get_aspect().template get<_insert_to_container_>()(t, std::make_pair(key, key), parr);
  }
};

}}

#endif
