//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_AD_ALLOCATE_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_AD_ALLOCATE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

#include <fas/typemanip/type2type.hpp>
#include <stdlib.h>
namespace vset { namespace memory{ namespace fsb{

struct ad_allocate
{
  template<typename T, typename Pointer>
  Pointer operator()(T& t, fas::type2type<Pointer>, size_t /*num*/ = 1, void *  /*hint*/ = 0 ) const
  {
    return _<Pointer>(t);
  }

private:
  
  template<typename Pointer, typename T>
  Pointer _(T& t) const
  {
    typedef Pointer pointer;
    typedef ::vset::buffer::_size_      _buffer_size_;
    typedef ::vset::buffer::_data_      _buffer_data_;
    typedef ::vset::buffer::_data_type_ _buffer_data_type_;
    
    typedef typename T::aspect::template advice_cast<_chain_type_>::type chain_type;
    typedef typename T::aspect::template advice_cast< _buffer_data_type_ >::type data_type;
   
    if ( t.get_aspect().template get< _buffer_size_ >()(t) == 0 )
      t.get_aspect().template get<_acquire_>()(t);

    data_type data = t.get_aspect().template get<_buffer_data_>()(t);
    chain_type* chn = reinterpret_cast<chain_type*>(data);
    pointer p( &t );
    p.set_address( chn->mark() );
    if (!p)
    {
      t.get_aspect().template get<_acquire_>()(t);
      data = t.get_aspect().template get<_buffer_data_>()(t);
      chn = reinterpret_cast<chain_type*>(data);
      p.set_address(chn->mark());
      if (!p)
        abort();
    }
    return p;
  }
};

}}}

#endif
