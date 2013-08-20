//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_ASPECT_VALUE_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_ASPECT_VALUE_HPP

#include <vset/memory/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

#include <vset/memory/fsb/chunk.hpp>
#include <vset/memory/fsb/chain.hpp>
#include <fas/aop.hpp>

namespace vset { namespace memory{ namespace fsb{

template<typename T>
struct aspect_value: fas::aspect< typename fas::type_list_n<
  fas::type_advice< _value_type_, T>,
  fas::type_advice< _chunk_type_, chunk<T> >,
  fas::type_advice< _chain_type_, chain<T, chunk > >
>::type > {};

}}}

#endif
