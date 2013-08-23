//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_ASPECT_MEMORY_HPP
#define VSET_VTREE_ASPECT_ASPECT_MEMORY_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <vset/memory/provider.hpp>
#include <vset/memory/allocator.hpp>
#include <fas/aop.hpp>
#include <fas/mp.hpp>


namespace vset{ namespace vtree{


template<typename Array, template<typename> class MemmoryStreategy >
struct aspect_memory: fas::aspect< typename fas::type_list_n<
  fas::type_advice<_array_type_, Array >,
  fas::advice<_allocator_, fas::provider< fas::w< memory::allocator< memory::provider< fas::_ > > > > >,
  fas::value_advice< _size_, size_t>,
  MemmoryStreategy<Array>
>::type> {};

}}

#endif
