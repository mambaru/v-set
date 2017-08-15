//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_MMAP_AD_CLEAR_HPP
#define VSET_VSET_BUFFER_PERSISTENT_MMAP_AD_CLEAR_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>


namespace vset { namespace buffer { namespace persistent{ namespace mmap{

struct ad_clear
{
  template<typename T>
  void operator()( T& t)
  {
    t.get_aspect().template get<_size_value_>() = 0;
    t.get_aspect().template get<_head_>()(t)->reset();
    //t.get_aspect().template get<_head_>()(t)->set_size(0);
    //t.get_aspect().template get<_head_>()(t)->set_capacity(0);
    //t.get_aspect().template get<_sync_>()(t, 0, 0, true );
  }
};

}}}}

#endif
