//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_BUFFER_BASE_HPP
#define VSET_VSET_BUFFER_BUFFER_BASE_HPP

#include <fas/aop.hpp>
#include <vset/buffer/tags.hpp>
namespace vset { namespace buffer{

template<typename A = fas::aspect<> >
class buffer_base
  : public fas::aspect_class<A>
{
  typedef fas::aspect_class<A> super;
public:

  typedef typename super::aspect::template advice_cast<_size_type_>::type size_type;
  typedef typename super::aspect::template advice_cast<_data_type_>::type data_type;

  ~buffer_base()
  {
    _constructor(*this);
  }
  
  buffer_base()
  {
    _destructor(*this);
  }

  template<typename T>
  void _constructor(T& t)
  {
    t.get_aspect().template getg< _destructor_ >()(t);
  }

  template<typename T>
  void _destructor(T& t)
  {
    t.get_aspect().template getg< _destructor_ >()(t);
  }

  template<typename T>
  data_type _data(T& t)
  {
    return t.get_aspect().template get<_data_>()(t);
  }

  template<typename T>
  data_type _data(T& t) const
  {
    return t.get_aspect().template get<_data_>()(t);
  }

  template<typename T>
  size_type _size(T& t) const
  {
    return t.get_aspect().template get<_size_>()(t);
  }

  template<typename T>
  size_type _capacity(T& t) const
  {
    return t.get_aspect().template get<_capacity_>()(t);
  }

  template<typename T>
  void _resize(T& t, size_type size)
  {
    t.get_aspect().template get<_resize_>()(t, size);
  }

  template<typename T>
  void _truncate(T& t, size_type size)
  {
    t.get_aspect().template get<_truncate_>()(t, size);
  }

  
  template<typename T>
  void _reserve(T& t, size_type size)
  {
    t.get_aspect().template get<_reserve_>()(t, size);
  }

  template<typename T>
  void _clear(T& t)
  {
    t.get_aspect().template get<_clear_>()(t);
  }
};

}}

#endif
