//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_AD_GET_BY_OFFSET_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_AD_GET_BY_OFFSET_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

namespace vset { namespace memory{ namespace fsb{


struct ad_ptr_by_offset
{
  template<typename T>
  typename T::aspect::template advice_cast<_value_type_>::type*
  operator()(T& t, size_t offset) const
  {
    typedef ::vset::buffer::_data_      _buffer_data_;
    typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
    return 
      const_cast<value_type*>(
        reinterpret_cast<const value_type*>( 
          t.get_aspect().template get<_buffer_data_>()(t) + 
          t.get_aspect().template get<_pos2off_>()(t, offset)
        )
      );
  }

};
 

}}}

#endif
