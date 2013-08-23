//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_ALLOCATORS_MMAP_ALLOCATOR_HPP
#define VSET_ALLOCATORS_MMAP_ALLOCATOR_HPP

#include <fas/typemanip/empty_type.hpp>
#include <vset/vtree/strategy.hpp>

namespace vset{

template<size_t N>
struct mmap_allocator
{
  typedef fas::empty_type aspect_maker;
  
  template<typename V, typename C>
  struct apply
  {
    typedef vtree::strategy::vtree_fsb_mmap<V, C, N> type;
  };
};

}

#endif
