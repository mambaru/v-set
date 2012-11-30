//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_MEMORY_MANAGER_BASE_HPP
#define VSET_MEMORY_MANAGER_BASE_HPP

#include <cstddef>

#include <vset/memory/tags.hpp>
#include <fas/typemanip/type2type.hpp>
#include <fas/aop.hpp>
#include <iostream>

namespace vset { namespace memory{

template<typename A >
class manager_base
  : public fas::aspect_class<A>
{
  typedef fas::aspect_class<A> super;
  typedef manager_base<A> self;
protected:
  typedef typename super::aspect::template advice_cast<_value_type_>::type value_type;
  
  typedef typename super::aspect::template advice_cast<_buffer_provider_>::type
                                ::type buffer_metatype;
                                
  typedef typename super::aspect::template advice_cast<_pointer_provider_>::type
                                ::type pointer_metatype;
                                
  typedef typename fas::apply<buffer_metatype, self>::type buffer_type;
  typedef typename fas::apply<pointer_metatype, value_type, self >::type pointer;
  typedef typename fas::apply<pointer_metatype, const value_type, const self >::type const_pointer;

  typedef size_t size_type;

  manager_base(){}

  template<typename T>
  pointer _begin(T& t)
  {
    return t.get_aspect().template get<_begin_>()(t, fas::type2type<pointer>() );
  }

  template<typename T>
  const_pointer _begin(T& t) const
  {
    return t.get_aspect().template get<_begin_>()(t, fas::type2type<const_pointer>() );
  }

  template<typename T>
  pointer _end(T& t)
  {
    return t.get_aspect().template get<_end_>()(t, fas::type2type<pointer>() );
  }

  template<typename T>
  const_pointer _end(T& t) const
  {
    return t.get_aspect().template get<_end_>()(t, fas::type2type<const_pointer>() );
  }

  template<typename T>
  pointer _allocate(T& t, size_t num, void *  hint = 0)
  {
    return t.get_aspect().template get<_allocate_>()(t, fas::type2type<pointer>(), num, hint );
  }

  template<typename T>
  void _deallocate(T& t, pointer ptr, size_type num)
  {
    return t.get_aspect().template get<_deallocate_>()(t, ptr, num );
  }

  template<typename T>
  buffer_type _buffer(T& t)
  {
    return buffer_type(&t);
  }

  template<typename T>
  const buffer_type _buffer(T& t) const
  {
    return buffer_type(&t);
  }
};

}}

#endif
