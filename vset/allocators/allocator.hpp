//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_ALLOCATOR_ALLOCATOR_HPP
#define VSET_ALLOCATOR_ALLOCATOR_HPP

#include <fas/typemanip/empty_type.hpp>
#include <vset/vtree/strategy.hpp>
#include <fas/aop/aspect.hpp>

namespace vset{

template<size_t N, typename A = fas::aspect<> >
struct allocator
{
  typedef fas::empty_type aspect_maker;
  
  template<typename V, typename C>
  struct apply
  {
    typedef typename fas::merge_aspect< A, vtree::strategy::vtree_std_alloc<V, C, N> >::type type;
  };
};
  
}

#endif
