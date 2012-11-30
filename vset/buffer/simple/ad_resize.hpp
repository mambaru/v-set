//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_SIMPLE_AD_RESIZE_HPP
#define VSET_VSET_BUFFER_SIMPLE_AD_RESIZE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/simple/tags.hpp>

namespace vset { namespace buffer { namespace simple{

struct ad_resize
{
  template<typename T>
  void operator()( T& t, typename T::aspect::template advice_cast<_size_type_>::type size )
  {
    t.get_aspect().template get<_container_>().resize(size);
  }
};

}}}

#endif
