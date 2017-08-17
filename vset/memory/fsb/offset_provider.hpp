//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_OFFSET_PROVIDER_HPP
#define VSET_VSET_MEMORY_FSB_OFFSET_PROVIDER_HPP

#include <vset/memory/fsb/tags.hpp>
#include <cassert>
#include <stdexcept>

namespace vset { namespace memory{ namespace fsb{

template<typename T>
class offset_provider
{
public:
  typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  
  offset_provider()
    : _target(0)
  {}
  
  offset_provider(T* off, size_t=static_cast<size_t>(-1))
    : _target(off)
  {}

  const value_type* get(size_t off) const
  {
    assert(_target!=0);
    return _target->get_aspect().template get<_ptr_by_offset_>()(*_target, off);
  }

  value_type* get(size_t off)
  {
    assert(_target!=0);
    return _target->get_aspect().template get<_ptr_by_offset_>()(*_target, off);
  }

  size_t offset(value_type* p) const
  {
    assert(_target!=0);
    return _target->get_aspect().template get<_offset_by_ptr_>()(*_target, p);
  }

  size_t next(size_t off, size_t cnt = 1) const
  {
    assert(_target!=0);
    return _target->get_aspect().template get<_next_offset_>()(*_target, off, cnt);
  }

  size_t pred(size_t off, size_t cnt = 1) const
  {
    assert(_target!=0);
    return _target->get_aspect().template get<_pred_offset_>()(*_target, off, cnt);
  }

  bool operator == (const offset_provider<T> right) const
  {
    return _target==right._target;
  }

private:
  T *_target;
};


}}}

#endif
