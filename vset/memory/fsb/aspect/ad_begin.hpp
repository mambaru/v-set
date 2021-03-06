//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_AD_BEGIN_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_AD_BEGIN_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

#include <fas/typemanip/type2type.hpp>

namespace vset { namespace memory{ namespace fsb{

struct ad_begin
{
  template<typename T, typename Pointer>
  Pointer operator()(T& t, fas::type2type<Pointer> ) const
  {
    return begin_<Pointer>(t);
  }

private:

  template<typename Pointer, typename T>
  Pointer begin_(T& t) const
  {
    typedef ::vset::buffer::_size_      _buffer_size_;
    typedef ::vset::buffer::_data_type_ _buffer_data_type_;
    typedef ::vset::buffer::_data_      _buffer_data_;
    typedef Pointer pointer;

    if ( t.get_aspect().template get< _buffer_size_ >()(t) == 0 )
    {
      return pointer( &t );
    }

    typedef typename T::aspect::template advice_cast<_chain_type_>::type chain_type;
    typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
    typedef typename T::aspect::template advice_cast<_buffer_data_type_>::type data_type;

    data_type data = const_cast<data_type>( t.get_aspect().template get<_buffer_data_>()(t) );

    chain_type* chn = static_cast<chain_type*>( static_cast<void*>(data));

    if ( value_type* value = chn->first_value() )
    {
      return pointer( &t, t.get_aspect().template get<_offset_by_ptr_>()(t, value) );
    }

    return pointer( &t );
  }
};

}}}

#endif
