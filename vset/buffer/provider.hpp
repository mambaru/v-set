//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_BUFFER_PROVIDER_HPP
#define VSET_BUFFER_PROVIDER_HPP

#include <vset/buffer/tags.hpp>

namespace vset { namespace buffer{

template<typename T>
class provider
{
public:
  typedef typename T::aspect::template advice_cast<_data_type_>::type data_type;
  typedef typename T::aspect::template advice_cast<_size_type_>::type size_type;

  explicit provider(T* target)
    : _target(target)
  {
  }

  size_type size() const
  {
    return _target->get_aspect().template get<_size_>()(*_target);
  }

  size_type capacity() const
  {
    return _target->get_aspect().template get<_capacity_>()(*_target);
  }

  data_type data()
  {
    return _target->get_aspect().template get<_data_>()(*_target);
  }

  const data_type data() const
  {
    return _target->get_aspect().template get<_data_>()(*_target);
  }

  void clear()
  {
    _target->get_aspect().template get<_clear_>()(*_target);
  }
  
  void resize(size_type s)
  {
    _target->get_aspect().template get<_resize_>()(*_target, s);
  }

  void truncate(size_type s)
  {
    _target->get_aspect().template get<_truncate_>()(*_target, s);
  }

  void reserve( size_type s)
  {
    _target->get_aspect().template get<_reserve_>()(*_target, s);
  }

protected:
  
  T* _target;
};

}}

#endif
