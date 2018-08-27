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

/**
 * @brief Интерфейс менеждера памяти
 * @tparam S стратегия 
 * @tparam A аспект
 * @details доступные стратегии:
 *    * vset::memory::strategy::fsb_mmap
 *    * vset::memory::strategy::fsb_inmem
 *    * vset::memory::strategy::fsb_filesync
 */
template<typename S, typename A = fas::aspect<> >
class manager
  : public manager_base< typename fas::merge_aspect<A, S>::type >
{
  typedef manager_base< typename fas::merge_aspect<A, S>::type > super;
public:
  typedef manager<S, A> self;
  /** Тип размещаемого объекта */
  typedef typename super::value_type    value_type;
  /** Буфер, где рамещаются объекты value_type*/
  typedef typename super::buffer_type   buffer_type;
  /** Указатель vset::memory::offset_pointer */
  typedef typename super::pointer       pointer;
  /** Константный vset::memory::offset_pointer */
  typedef typename super::const_pointer const_pointer;
  
  typedef value_type& reference;
  typedef const value_type& const_reference;

  /** Беззнаковый целочисленный тип (size_t) */
  typedef size_t size_type;
  /** Знаковый целочисленный тип (std::ptrdiff_t) */
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
