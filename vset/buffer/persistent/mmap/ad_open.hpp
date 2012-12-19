//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_MMAP_AD_OPEN_HPP
#define VSET_VSET_BUFFER_PERSISTENT_MMAP_AD_OPEN_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>
#include <vset/buffer/persistent/mmap/tags.hpp>
#include <string.h>
#include <string>

#include <unistd.h>
#include <sys/mman.h>


namespace vset { namespace buffer { namespace persistent{ namespace mmap{

struct ad_open
{
  ad_open(){}
  
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
    
    bool is_created = file_size < sizeof(head_type);
    if ( is_created  )
    {
      file_size = sizeof(head_type);
      t.get_aspect().template get<_resize_file_>()(t, file_size);
      /*t.get_aspect().template get<_buffer_size_>() = buffer_size;
      t.get_aspect().template get<_buffer_>() = data;
      head->set_size(0);
      head->set_capacity(0);
      */
      /*t.get_aspect().template get<_seek_set_>()(t, file_size);
      t.get_aspect().template get<_write_file_>()(t, "", 1);
      */
    }

    char* data = (char*)::mmap(
      NULL,
      file_size,
      PROT_READ | PROT_WRITE, MAP_SHARED | MAP_NORESERVE |  MAP_POPULATE,
      t.get_aspect().template get<_descriptor_>(),
      0
    );

    if ( data == MAP_FAILED)
      throw;

    t.get_aspect().template get<_buffer_size_>() = file_size;
    t.get_aspect().template get<_buffer_>() = data;
    head_type* head = t.get_aspect().template get<_head_>()(t);

    if ( is_created )
      *head = head_type();

    t.get_aspect().template get<_size_value_>() = head->size();
    t.get_aspect().template get<_capacity_value_>() = head->capacity();
    
    
    /*
    char* addr = (char*)::mmap(NULL, capacity + super::offset(), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_NORESERVE |  MAP_POPULATE, _fd, 0);

    if (addr == MAP_FAILED)
    {*/


    /*
    size_t file_size = t.get_aspect().template get<_file_size_>()(t);
    typedef typename T::aspect::template advice_cast<_head_type_>::type head_type;
    head_type* head = 0;
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
    */
  }
/*private:
  size_t _buffer_size;*/
};

}}}}

#endif
