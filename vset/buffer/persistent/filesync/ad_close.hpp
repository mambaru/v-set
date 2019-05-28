//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_FILESYNC_AD_CLOSE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_FILESYNC_AD_CLOSE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>
#include <fas/system/nullptr.hpp>

namespace vset { namespace buffer { namespace persistent{ namespace filesync{

struct ad_close
{
  template<typename T>
  void operator()( T& t)
  {
    t.get_aspect().template get<_close_file_>()(t);
    delete[] t.get_aspect().template get<_buffer_>();
    t.get_aspect().template get<_buffer_>() = fas_nullptr;
    t.get_aspect().template get<_buffer_size_>() = 0;
    t.get_aspect().template get<_size_value_>() = 0;
    t.get_aspect().template get<_capacity_value_>() = 0;
    t.get_aspect().template get<_file_name_>().clear();
    
  }
};

}}}}

#endif
