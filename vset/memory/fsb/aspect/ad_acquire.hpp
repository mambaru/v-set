//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_ALLOCATOR_ASPECT_AD_ACQUIRE_HPP
#define VSET_VSET_ALLOCATOR_ASPECT_AD_ACQUIRE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>
#include <fas/typemanip/type2type.hpp>

namespace vset { namespace memory{ namespace fsb{

struct ad_acquire
{
  template<typename T>
  bool operator()(T& t) const
  {
    typedef ::vset::buffer::_size_      _buffer_size_;
    typedef ::vset::buffer::_resize_    _buffer_resize_;
    typedef ::vset::buffer::_data_type_ _buffer_data_type_;
    typedef ::vset::buffer::_data_      _buffer_data_;

    typedef typename T::aspect::template advice_cast<_chain_type_>::type chain_type;
    typedef typename T::aspect::template advice_cast<_chunk_type_>::type chunk_type;
    typedef typename T::aspect::template advice_cast< _buffer_data_type_ >::type data_type;

    size_t offset = t.get_aspect().template get< _buffer_size_ >()(t);
    
    size_t new_size = (offset == 0)
      ? sizeof(chain_type) + sizeof(chunk_type)
      : offset + sizeof(chunk_type);

    t.get_aspect().template get< _buffer_resize_ >()(t, new_size);

    data_type data = t.get_aspect().template get<_buffer_data_>()(t);

    chain_type* ch = (offset == 0)
      ? new (data) chain_type
      : reinterpret_cast<chain_type*>(data);

    ch->acquire(1);

    offset == 0
      ? new ( data + sizeof(chain_type) )chunk_type
      : new ( data + offset )chunk_type;
    
    return true;
  }
};


}}}

#endif
