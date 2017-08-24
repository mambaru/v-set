//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_ALLOCATOR_CHAIN_ALLOCATOR_HPP
#define VSET_VSET_ALLOCATOR_CHAIN_ALLOCATOR_HPP

#include <cstddef>
#include <vset/memory/manager_base.hpp>
#include <vset/memory/strategy.hpp>

namespace vset { namespace memory{

template<typename A >
class manager
  : public manager_base<A>
{
  typedef manager_base<A> super;
public:
  typedef manager<A> self;
  typedef typename super::value_type    value_type;
  typedef typename super::buffer_type   buffer_type;
  typedef typename super::pointer       pointer;
  typedef typename super::const_pointer const_pointer;
  
  typedef value_type& reference;
  typedef const value_type& const_reference;

  typedef size_t size_type;
  typedef std::ptrdiff_t difference_type;

  pointer begin()
  {
    return super::_begin(*this);
  }

  const_pointer begin() const
  {
    return super::_begin(*this);
  }

  pointer end()
  {
    return super::_end(*this);
  }

  const_pointer end() const
  {
    return super::_end(*this);
  }
  
  pointer allocate(size_t num, void *  hint = 0)
  {
    return super::_allocate(*this, num, hint);
  }

  void deallocate(pointer ptr, size_type num)
  {
    return super::_deallocate(*this, ptr, num);
  }

  size_type count() const
  {
    return super::_count(*this);
  }
  
  bool empty() const
  {
    return this->begin() == this->end();
  }

  size_type capacity() const
  {
    return super::_capacity(*this);
  }
  
  buffer_type buffer()
  {
    return super::_buffer(*this);
  }

  const buffer_type buffer() const
  {
    return super::_buffer(*this);
  }

};

}}

#endif
