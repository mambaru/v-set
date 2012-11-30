//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_FILESYNC_AD_RESIZE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_FILESYNC_AD_RESIZE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>
#include <iostream>
#include <stdlib.h>


namespace vset { namespace buffer { namespace persistent{ namespace filesync{

struct ad_resize
{
  template<typename T>
  void operator()( T& t, size_t size)
  {
    
    size_t capacity = t.get_aspect().template get<_capacity_>()(t);

    if ( size <= capacity )
    {
      t.get_aspect().template get<_head_>()(t)->set_size(size);
      t.get_aspect().template get<_size_value_>()=size;
      return;
    }
    t.get_aspect().template get<_head_>()(t)->set_size(size);
    t.get_aspect().template get<_size_value_>()=size;
    if (size > capacity)
    {
      t.get_aspect().template get<_head_>()(t)->set_capacity(size);
      t.get_aspect().template get<_capacity_value_>()=size;
    }
    t.get_aspect().template get<_sync_>()(t);
    t.get_aspect().template get<_open_>()(t);
  }
};

}}}}

#endif
