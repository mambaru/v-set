//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_BASIC_AD_SEEK_SET_HPP
#define VSET_VSET_BUFFER_PERSISTENT_BASIC_AD_SEEK_SET_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>

#include <fcntl.h>

namespace vset { namespace buffer { namespace persistent{

struct ad_seek_set
{
  template<typename T>
  void operator()( T& t, size_t size )
  {
    ::lseek( t.get_aspect().template get<_descriptor_>() , size, SEEK_SET);
  }
};

}}}

#endif
