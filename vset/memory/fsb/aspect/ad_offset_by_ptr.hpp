//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_ALLOCATOR_ASPECT_AD_OFFSET_BY_PTR_HPP
#define VSET_VSET_ALLOCATOR_ASPECT_AD_OFFSET_BY_PTR_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

namespace vset { namespace memory{ namespace fsb{

struct ad_offset_by_ptr
{
  template<typename T>
  size_t operator()(T& t, typename T::aspect::template advice_cast<_value_type_>::type* ptr) const
  {
    if ( ptr==0 )
      return static_cast<size_t>(-1);

    typedef ::vset::buffer::_data_      _buffer_data_;
    typedef ::vset::buffer::_data_type_ _buffer_data_type_;

    typedef typename T::aspect::template advice_cast<_buffer_data_type_>::type data_type;

    data_type data = t.get_aspect().template get<_buffer_data_>()(t);
    return reinterpret_cast<data_type>(ptr) - data;
  }
};

}}}

#endif
