//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_MMAP_AD_RESERVE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_MMAP_AD_RESERVE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>


namespace vset { namespace buffer { namespace persistent{ namespace mmap{

struct ad_reserve
{
  template<typename T>
  void operator()( T& t, size_t size)
  {
    size_t capacity = t.get_aspect().template get<_capacity_>()(t);
    if ( size <= capacity )
      return;

    t.get_aspect().template get<_truncate_>()(t, size);
  }
};

}}}}

#endif
