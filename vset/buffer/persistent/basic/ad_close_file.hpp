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


namespace vset { namespace buffer { namespace persistent{

struct ad_close_file
{
  template<typename T>
  void operator()( T& t )
  {
    if ( t.get_aspect().template get<_descriptor_>() != -1 )
    {
      ::close( t.get_aspect().template get<_descriptor_>() );
      t.get_aspect().template get<_descriptor_>() =-1;
      t.get_aspect().template get<_file_status_>() = false;
    }
  }

};

}}}

#endif
