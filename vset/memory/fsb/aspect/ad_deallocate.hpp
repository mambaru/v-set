//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_AD_DEALLOCATE_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_AD_DEALLOCATE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

namespace vset { namespace memory{ namespace fsb{

struct ad_deallocate
{
  template<typename T, typename Pointer>
  void operator()(T& t, Pointer ptr, size_t num = 1) const
  {
    typedef ::vset::buffer::_data_      _buffer_data_;
    typedef ::vset::buffer::_data_type_ _buffer_data_type_;

    //typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
    typedef typename T::aspect::template advice_cast<_chain_type_>::type chain_type;
    typedef typename T::aspect::template advice_cast<_buffer_data_type_>::type data_type;

    for (;num!=0; --num)
    {
      data_type data = t.get_aspect().template get<_buffer_data_>()(t);
      chain_type* chn = static_cast<chain_type*>( static_cast<void*>(data) );
      chn->free( ptr.get_address() );
      --ptr;
    }

  }
};

}}}

#endif
