//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_SIMPLE_AD_RESERVE_HPP
#define VSET_VSET_BUFFER_SIMPLE_AD_RESERVE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/simple/tags.hpp>

namespace vset { namespace buffer { namespace simple{

struct ad_reserve
{
  template<typename T>
  void operator()( T& t, typename T::aspect::template advice_cast<_size_type_>::type size )
  {
    t.get_aspect().template get<_container_>().reserve(size);
  }
};

}}}

#endif
