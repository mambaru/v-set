//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_MMAP_AD_TRUNCATE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_MMAP_AD_TRUNCATE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>
#include <unistd.h>
#include <sys/mman.h>


namespace vset { namespace buffer { namespace persistent{ namespace mmap{

struct ad_truncate
{
  template<typename T>
  void operator()( T& t, size_t size)
  {
    typedef typename T::aspect::template advice_cast<_head_type_>::type head_type;
    size_t capacity = t.get_aspect().template get<_capacity_>()(t);
    size_t oldsize = sizeof(head_type) + capacity;
    size_t newsize = sizeof(head_type) + size;

    t.get_aspect().template get<_resize_file_>()(t, newsize);
    char* olddata = t.get_aspect().template get<_buffer_>();
    char* data = (char*) ::mremap( olddata, oldsize, newsize, MREMAP_MAYMOVE);
    if ( data == MAP_FAILED)
      throw std::runtime_error(strerror(errno));

    if ( oldsize < newsize )
      ::memset( data + oldsize, 0, newsize - oldsize);
    
    t.get_aspect().template get<_buffer_>() = data;
    t.get_aspect().template get<_buffer_size_>() = newsize;
    t.get_aspect().template get<_head_>()(t)->set_capacity(size);
    t.get_aspect().template get<_capacity_value_>() = size;

    if ( size <= capacity )
    {
      t.get_aspect().template get<_head_>()(t)->set_size(size);
      t.get_aspect().template get<_size_value_>() = size;
    }
    t.get_aspect().template get<_sync_>()(t, 0, 0);
  }
};

}}}}

#endif
