//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_MMAP_AD_CLOSE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_MMAP_AD_CLOSE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>


namespace vset { namespace buffer { namespace persistent{ namespace mmap{

struct ad_close
{
  template<typename T>
  void operator()( T& t)
  {
    char *data = t.get_aspect().template get<_buffer_>();
    size_t size = t.get_aspect().template get<_buffer_size_>();
    ::munmap( data, size );
    t.get_aspect().template get<_close_file_>()(t);
    t.get_aspect().template get<_buffer_>() = 0;
    t.get_aspect().template get<_buffer_size_>() = 0;
    t.get_aspect().template get<_size_value_>() = 0;
    t.get_aspect().template get<_capacity_value_>() = 0;
    t.get_aspect().template get<_file_name_>().clear();
  }
};

}}}}

#endif
