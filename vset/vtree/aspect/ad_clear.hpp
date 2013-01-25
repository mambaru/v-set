//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_CLEAR_HPP
#define VSET_VTREE_ASPECT_AD_CLEAR_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <string.h>
#include <stdlib.h>

namespace vset{ namespace vtree{

struct ad_clear
{
  template<typename T>
  void operator()(T& t)
  {
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator container_iterator;

    container_type& container = t.get_container();
    container_iterator beg = container.begin();
    container_iterator end = container.end();

    for (;beg!=end; ++beg)
      t.get_allocator().deallocate(beg->second, 1);

    t.get_aspect().template get<_size_>() = 0;
    container.clear();
  }
};

}}

#endif
