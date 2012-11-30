//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_FILESYNC_AD_SYNC_HPP
#define VSET_VSET_BUFFER_PERSISTENT_FILESYNC_AD_SYNC_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>
#include <iostream>

namespace vset { namespace buffer { namespace persistent{ namespace filesync{

struct ad_sync
{
  template<typename T>
  size_t operator()( T& t, size_t offset, size_t limit) const
  {
    return _write(t, offset, limit);
  }

  template<typename T>
  size_t operator()( T& t) const
  {
    size_t offset = 0;
    size_t limit = t.get_aspect().template get<_size_>()(t);
    return _write(t, offset, limit);
  }

private:

  template<typename T>
  size_t _write( T& t, size_t offset, size_t limit) const
  {
    typedef typename T::aspect::template advice_cast<_head_type_>::type head_type;
    size_t buffer_size = t.get_aspect().template get<_buffer_size_>();

    if ( buffer_size < sizeof(head_type) )
      throw;

    const head_type* head = t.get_aspect().template get<_head_>()(t);
    if ( head==0 )
      throw;

    size_t head_offset = head->offset();
    if ( buffer_size < head->offset() )
      throw;

    size_t size = head->size();

    if ( offset >= size && offset!=0 )
      return 0;

    if ( offset + limit > size )
      limit = size - offset;

    size_t real_size = limit;
    size_t real_offset = offset;
    
    if ( offset==0 )
      real_size += head_offset;
    else
      real_offset += head_offset;

    if ( offset + real_size >  buffer_size)
      real_size = buffer_size - offset;

    const char *buff = t.get_aspect().template get<_buffer_>();
    
    t.get_aspect().template get<_write_file_>()(
      t,
      buff + real_offset,
      real_size,
      real_offset
    );

    return real_offset + real_size >= buffer_size ? 0 : offset + limit;
  }


};

}}}}

#endif
