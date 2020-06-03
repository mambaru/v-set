//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_AD_PRED_OFFSET_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_AD_PRED_OFFSET_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

namespace vset { namespace memory{ namespace fsb{

struct ad_pred_offset
{
  template<typename T>
  size_t operator()(T& t, size_t offset, size_t count) const
  {
    typedef ::vset::buffer::_data_      _buffer_data_;
    typedef ::vset::buffer::_data_type_ _buffer_data_type_;

    typedef typename T::aspect::template advice_cast<_chain_type_>::type chain_type;
    typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
    typedef typename T::aspect::template advice_cast<_buffer_data_type_>::type data_type;

    data_type data = t.get_aspect().template get<_buffer_data_>()(t);
    chain_type* chn = static_cast<chain_type*>( static_cast<void*>(data) );

    if ( offset == static_cast<size_t>(-1) )
    {
      if ( value_type* current = chn->last_value() )
      {
        offset = t.get_aspect().template get<_offset_by_ptr_>()(t, current);
      }
    }
    else for (size_t i = 0 ; i < count ; ++i)
    {
      if ( value_type* current = chn->pred_value( t.get_aspect().template get<_ptr_by_offset_>()(t, offset) ) )
      {
        offset = t.get_aspect().template get<_offset_by_ptr_>()(t, current);
      }
      else
      {
        return static_cast<size_t>(-1);
      }
    }

    return offset;
  }
};

}}}

#endif
