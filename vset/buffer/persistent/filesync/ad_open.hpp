//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_FILESYNC_AD_OPEN_HPP
#define VSET_VSET_BUFFER_PERSISTENT_FILESYNC_AD_OPEN_HPP

#include <fas/system/nullptr.hpp>
#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>
#include <vset/buffer/persistent/filesync/tags.hpp>
#include <string.h>
#include <string>

namespace vset { namespace buffer { namespace persistent{ namespace filesync{

struct ad_open
{
  template<typename T>
  void operator()( T& t)
  {
    std::string file_name = t.get_aspect().template get<_file_name_>();
    this->operator()(t, file_name );
  }
  
  template<typename T>
  void operator()( T& t, const std::string& file_name)
  {
    t.get_aspect().template get<_close_>()(t);
    t.get_aspect().template get<_file_name_>() = file_name;
    t.get_aspect().template get<_open_file_>()(t);
    size_t file_size = t.get_aspect().template get<_file_size_>()(t);
    typedef typename T::aspect::template advice_cast<_head_type_>::type head_type;
    head_type* head = fas_nullptr;
    if ( file_size < sizeof(head_type) )
    {
      size_t buffer_size = sizeof(head_type);
      char *data = new char[sizeof(head_type)];
      head = new (data) head_type();
      t.get_aspect().template get<_buffer_size_>() = buffer_size;
      t.get_aspect().template get<_buffer_>() = data;
      head->set_size(0);
      head->set_capacity(0);
    }
    else
    {
      head_type header;
      t.get_aspect().template get<_read_file_>()(t, reinterpret_cast<char*>(&header), sizeof(head_type), 0);
      size_t buffer_size = header.offset() + header.capacity();
      char *data = new char[buffer_size];
      memset(data, 0, buffer_size);
      memcpy(data, &header, sizeof(head_type) );
      t.get_aspect().template get<_buffer_size_>() = buffer_size;
      t.get_aspect().template get<_read_file_>()(t, data + header.offset(), header.size(), header.offset());
      t.get_aspect().template get<_buffer_>() = data;
      head = t.get_aspect().template get<_head_>()(t);
    }
    t.get_aspect().template get<_size_value_>() = head->size();
    t.get_aspect().template get<_capacity_value_>() = head->capacity();
  }
};

}}}}

#endif
