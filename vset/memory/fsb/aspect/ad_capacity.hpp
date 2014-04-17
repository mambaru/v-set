//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_AD_CAPACITY_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_AD_CAPACITY_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

#include <fas/typemanip/type2type.hpp>

namespace vset { namespace memory{ namespace fsb{

struct ad_capacity
{
  template<typename T>
  size_t operator()(T& t ) const
  {
    typedef ::vset::buffer::_size_      _buffer_size_;

    if ( t.get_aspect().template get< _buffer_size_ >()(t) == 0 )
    {
      return 0;
    }

    typedef ::vset::buffer::_data_      _buffer_data_;
    typedef ::vset::buffer::_data_type_ _buffer_data_type_;
    
    typedef typename T::aspect::template advice_cast<_buffer_data_type_>::type data_type;

    data_type data = t.get_aspect().template get<_buffer_data_>()(t);

    typedef typename T::aspect::template advice_cast<_chain_type_>::type chain_type;
    chain_type* chn = reinterpret_cast<chain_type*>(data);
    
    return chn->capacity();
  }
};

}}}

#endif
