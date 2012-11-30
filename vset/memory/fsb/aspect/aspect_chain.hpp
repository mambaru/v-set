//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_ALLOCATOR_ASPECT_ASPECT_CHAIN_HPP
#define VSET_VSET_ALLOCATOR_ASPECT_ASPECT_CHAIN_HPP

#include <vset/buffer/persistent/basic/aspect_basic.hpp>
#include <vset/buffer/persistent/filesync/aspect_filesync.hpp>
#include <vset/persistent_buffer_proxy.hpp>
#include <vset/allocator/aspect/ad_begin.hpp>
#include <vset/allocator/aspect/ad_acquire.hpp>
#include <vset/allocator/aspect/ad_allocate.hpp>
#include <vset/allocator/aspect/ad_deallocate.hpp>
#include <vset/allocator/aspect/ad_get_by_offset.hpp>
#include <vset/allocator/aspect/ad_offset_by_ptr.hpp>
#include <vset/allocator/aspect/ad_next_offset.hpp>
#include <vset/allocator/aspect/tags.hpp>
#include <vset/allocator/offset_pointer.hpp>

#include <vset/allocator/chunk.hpp>
#include <vset/allocator/chain.hpp>

#include <fas/mp.hpp>
#include <fas/aop.hpp>

namespace vset { namespace allocator{

  /*
template<typename T>
struct value_type: fas::type_list_n<
  fas::type_advice< _value_type_, T>,
  fas::type_advice< _chunk_type_, chunk<T> >,
  fas::type_advice< _chain_type_, chain<T, chunk > >
>::type {};

struct aspect_chain1: fas::aspect< fas::type_list_n<
  fas::type_advice< _buffer_proxy_,  fas::w< vset::buffer::persistent_buffer_proxy< fas::_> > >,
  fas::type_advice< _pointer_type_,  fas::w< offset_pointer< fas::_, offset_proxy<fas::_> > > >,
  fas::advice<_begin_, ad_begin>,
  fas::advice<_acquire_, ad_acquire>,
  fas::advice<_allocate_, ad_allocate>,
  fas::advice<_deallocate_, ad_deallocate>,
  fas::advice<_get_by_offset_, ad_get_by_offset>,
  fas::advice<_offset_by_ptr_, ad_offset_by_ptr>,
  fas::advice<_next_offset_, ad_next_offset>
>::type> {};

struct aspect_chain: fas::aspect_merge<
  aspect_chain1,
  vset::buffer::persistent::filesync::aspect_filesync,
  vset::buffer::persistent::aspect_basic
>::type{};
*/
}}

#endif
