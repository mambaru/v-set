//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_ALLOCATOR_OFFSET_PROXY_HPP
#define VSET_VSET_ALLOCATOR_OFFSET_PROXY_HPP

#include <vset/memory/fsb/tags.hpp>
#include <cassert>
#include <stdexcept>

namespace vset { namespace memory{ namespace fsb{

template<typename T>
class offset_proxy
{
  typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
public:
  offset_proxy(): _offset(0) {}
  
  offset_proxy(T* offset): _offset(offset) {}

  const value_type* get(size_t offset) const
  {
    assert(_offset!=0);
    return _offset->get_aspect().template get<_get_by_offset_>()(*_offset, offset);
  }

  value_type* get(size_t offset)
  {
    assert(_offset!=0);
    return _offset->get_aspect().template get<_get_by_offset_>()(*_offset, offset);
  }

  size_t offset(value_type* p) const
  {
    assert(_offset!=0);
    return _offset->get_aspect().template get<_offset_by_ptr_>()(*_offset, p);
  }

  size_t next(size_t offset, size_t count = 1) const
  {
    assert(_offset!=0);
    return _offset->get_aspect().template get<_next_offset_>()(*_offset, offset, count);
  }

  size_t pred(size_t offset, size_t count = 1) const
  {
    assert(_offset!=0);
    throw std::domain_error("offset_proxy::pred not impl");
    // return _offset->get_aspect().template get<_next_offset_>()(*_offset, offset, count);
  }

  bool operator == (const offset_proxy<T> right) const
  {
    return _offset==right._offset;
  }

private:
  T *_offset;
};


}}}

#endif
