//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_STRATEGY_HPP
#define VSET_VTREE_STRATEGY_HPP

#include <vset/vtree/aspect/aspect_tree.hpp>
#include <vset/vtree/aspect/aspect_memory.hpp>
#include <vset/vtree/aspect/aspect_basic.hpp>
#include <vset/vtree/aspect/ad_restore.hpp>
#include <vset/memory/strategy.hpp>
#include <vset/sorted_array.hpp>

namespace vset{ namespace vtree{

template<typename Class>
struct creator // TODO: base type_advice и анологично для провайдера
{
  template<typename>
  struct apply
  {
    typedef Class type;
  };

  template<typename T>
  Class operator()(T& ) const
  {
    return Class();
  }
};
  
namespace strategy
{

template<typename V, typename Compare = std::less<V>, int N = 1024 >
struct vtree_fsb_mmap: fas::aspect_merge<
  aspect_tree<V, Compare>,
  aspect_memory< sorted_array< V, N, Compare >,  memory::strategy::fsb_mmap>,
  fas::advice< _restore_, ad_restore >,
  fas::group< buffer::persistent::_after_open_, _restore_ >,
  aspect_basic
>::type {};

template<typename V, typename Compare = std::less<V>, int N = 1024 >
struct vtree_fsb_filesync: fas::aspect_merge<
  aspect_tree<V, Compare>,
  aspect_memory< sorted_array< V, N, Compare >,  memory::strategy::fsb_filesync>,
  fas::advice< _restore_, ad_restore >,
  fas::group< buffer::persistent::_after_open_, _restore_ >,
  aspect_basic
>::type {};


template<typename V, typename Compare = std::less<V>, int N = 1024 >
struct vtree_fsb_inmem: fas::aspect_merge<
  aspect_tree<V, Compare>,
  aspect_memory< sorted_array< V, N, Compare >,  memory::strategy::fsb_inmem>,
  aspect_basic
>::type {};


template<typename V, typename Compare = std::less<V>, int N = 1024 >
struct vtree_std_alloc: fas::aspect_merge<
  aspect_tree<V, Compare>,
  fas::type_advice<_array_type_, sorted_array< V, N, Compare > >,
  fas::advice<_allocator_, creator< std::allocator< sorted_array< V, N, Compare > > > >,
  fas::value_advice< _size_, size_t>,
  aspect_basic
>::type {};

}

}}

#endif
