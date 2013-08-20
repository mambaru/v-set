//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_FILE_AD_BASIC_INIT_HPP
#define VSET_VSET_BUFFER_PERSISTENT_FILE_AD_BASIC_INIT_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>

namespace vset { namespace buffer { namespace persistent{ namespace file{

struct ad_basic_init
{
  template<typename T>
  void operator()( T& t )
  {
    t.get_aspect().template get<_descriptor_>() = -1;
  }
};

}}}}

#endif
