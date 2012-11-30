//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_SIMPLE_AD_CLEAR_HPP
#define VSET_VSET_BUFFER_SIMPLE_AD_CLEAR_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/simple/tags.hpp>

namespace vset { namespace buffer { namespace simple{

struct ad_clear
{
  template<typename T>
  void operator()( T& t)
  {
    return t.get_aspect().template get<_container_>().clear();
  }
};

}}}

#endif
