//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_ALLOCATOR_ASPECT_AD_GET_BY_OFFSET_HPP
#define VSET_VSET_ALLOCATOR_ASPECT_AD_GET_BY_OFFSET_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

namespace vset { namespace memory{ namespace fsb{
  
struct ad_get_by_offset
{
  template<typename T>
  typename T::aspect::template advice_cast<_value_type_>::type*
  operator()(T& t, size_t offset) const
  {
    typedef ::vset::buffer::_data_      _buffer_data_;
    typedef ::vset::buffer::_data_type_ _buffer_data_type_;

    typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
    typedef typename T::aspect::template advice_cast<_buffer_data_type_>::type data_type;

    data_type data = t.get_aspect().template get<_buffer_data_>()(t);
    return reinterpret_cast<value_type*>(data + offset);
  }

private:
};

}}}

#endif
