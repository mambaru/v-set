//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_BASIC_AD_READ_FILE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_BASIC_AD_READ_FILE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>

#include <fcntl.h>

namespace vset { namespace buffer { namespace persistent{

struct ad_read_file
{
  template<typename T>
  void operator()( T& t, char* data, size_t size, size_t offset )
  {
    if ( -1 == ::pread( t.get_aspect().template get<_descriptor_>() , data, size, offset) )
      throw std::domain_error(strerror(errno));
  }
};

}}}

#endif
