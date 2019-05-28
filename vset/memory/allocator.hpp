#ifndef MANAGED_ALLOCATOR_HPP
#define MANAGED_ALLOCATOR_HPP

#include <fas/system/nullptr.hpp>
#include <limits>
#include <cassert>

namespace vset { namespace memory{

template<typename Memory >
struct allocator
{
  typedef Memory memory_type;
  typedef typename memory_type::value_type value_type;
  typedef typename memory_type::pointer pointer;
  typedef typename memory_type::const_pointer const_pointer;
  
  typedef typename memory_type::reference       reference;
  typedef typename memory_type::const_reference const_reference;
  typedef typename memory_type::size_type size_type;
  typedef typename memory_type::difference_type difference_type;

  allocator()
    : _memory()
  {
  }
  
  explicit allocator(const memory_type& m)
    : _memory(m)
  {
  }

  template <typename U>
  struct rebind {
    typedef std::allocator<U> other;
  };

  pointer address(reference value ) const
  {
    return static_cast<char*>(&value) - _memory.data();
  }

  const_pointer address (const_reference value) const
  {
    return static_cast<char*>(&value) - _memory.data();
  }
  
  static size_type max_size () 
  {
    return 1;
  }

  pointer allocate (size_type num, void *  hint = fas_nullptr)
  {
    return this->_memory.allocate(num, hint);
  }

  static void construct (pointer p, const_reference value)
  {
    *p = value;
  }

  static void destroy (pointer p)
  {
    p->~T();
  }

  void deallocate (pointer p, size_type num)
  {
    this->_memory.deallocate(p, num);
  }

  memory_type memory() const
  {
    return this->_memory;
  }

  /*
  memory_type memory()
  {
    return this->_memory;
  }*/
  
private:
  memory_type _memory;
};

}}

#endif
