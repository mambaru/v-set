//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_BASIC_AD_CLOSE_FILE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_BASIC_AD_CLOSE_FILE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>

#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <errno.h>
#include <string.h>

namespace vset { namespace buffer { namespace persistent{ namespace file{

struct ad_close_file
{
  template<typename T>
  void operator()( T& t )
  {
    /// КОСТЫЛЬ
    if (  t.get_aspect().template get<_descriptor_>() == 0 )
      t.get_aspect().template get<_descriptor_>() = -1;

    if ( t.get_aspect().template get<_descriptor_>() != -1 )
    {
      int result = ::close( t.get_aspect().template get<_descriptor_>() );
      t.get_aspect().template get<_descriptor_>() = -1;
      t.get_aspect().template get<_file_status_>() = false;
      if ( result == -1 )
        throw std::domain_error(strerror(errno));
    }
  }

};

}}}}

#endif
