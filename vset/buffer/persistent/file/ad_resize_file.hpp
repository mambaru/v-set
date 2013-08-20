//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_FILE_AD_RESIZE_FILE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_FILE_AD_RESIZE_FILE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>
#include <sys/stat.h>
#include <string.h>
#include <stdexcept>
#include <errno.h>

namespace vset { namespace buffer { namespace persistent{ namespace file{

struct ad_resize_file
{
  template<typename T>
  void operator()( T& t, size_t size )
  {
    size_t cur_size = t.get_aspect().template get<_file_size_>()(t);
    if ( size == cur_size )
    {
      return;
    }

    if ( -1 == ftruncate( t.get_aspect().template get<_descriptor_>(), size) )
    {
      throw std::runtime_error(strerror(errno));
    }
  }
};

}}}}

#endif
