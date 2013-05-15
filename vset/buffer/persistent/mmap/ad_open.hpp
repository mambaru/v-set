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

    if ( is_created )
      ::memset(data, 0, file_size);

    if( ::madvise(data, 0, MADV_RANDOM) < 0 )
      throw;

    t.get_aspect().template get<_buffer_size_>() = file_size;
    t.get_aspect().template get<_buffer_>() = data;
    head_type* head = t.get_aspect().template get<_head_>()(t);

    if ( is_created )
      *head = head_type();

    t.get_aspect().template get<_size_value_>() = head->size();
    t.get_aspect().template get<_capacity_value_>() = head->capacity();
  }
};

}}}}

#endif
