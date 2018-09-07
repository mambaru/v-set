//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_FILESYNC_AD_TRUNCATE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_FILESYNC_AD_TRUNCATE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>
#include <unistd.h>

namespace vset { namespace buffer { namespace persistent{ namespace filesync{


struct ad_truncate
{
  template<typename T>
  void operator()( T& t, size_t s)
  {
    typedef typename T::aspect::template advice_cast<_head_type_>::type head_type;
    head_type* head = t.get_aspect().template get<_head_>()(t);
    size_t size = head->size();
    size_t data_offset = head->offset();

    t.get_aspect().template get<_head_>()(t)->set_capacity(s);
    if (s < size )
      t.get_aspect().template get<_head_>()(t)->set_size(s);

    t.get_aspect().template get<_sync_>()(t);
    std::string filename = t.get_aspect().template get<_file_name_>();
    t.get_aspect().template get<_close_>()(t);
    if ( -1 == ::truncate(filename.c_str(), static_cast<off_t>(data_offset + s) ) )
      throw std::runtime_error(strerror(errno));
    t.get_aspect().template get<_open_>()(t, filename);
  }
};


}}}}

#endif
