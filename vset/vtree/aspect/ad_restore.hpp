//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012, 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_RESTORE_HPP
#define VSET_VTREE_ASPECT_AD_RESTORE_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <utility>

namespace vset{ namespace vtree{

  struct ad_restore
{
  template<typename T>
  void operator()(T& t)
  {
    typedef typename T::allocator_type::memory_type::pointer pointer;
    pointer beg = t.get_allocator().memory().begin();
    pointer end = t.get_allocator().memory().end();
    for (;beg != end; ++beg)
    {
      t.get_aspect().template get<_insert_to_container_>()(t, std::make_pair( beg->front(), beg->back() ), beg);
      t.get_aspect().template get<_size_>() += beg->size();
    }
  }
};

}}

#endif
