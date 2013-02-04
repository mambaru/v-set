//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_ASPECT_POINTER_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_ASPECT_POINTER_HPP

#include <vset/memory/fsb/tags.hpp>

#include <vset/memory/fsb/aspect/ad_offset_by_ptr.hpp>
#include <vset/memory/fsb/aspect/ad_get_by_offset.hpp>
#include <vset/memory/fsb/aspect/ad_next_offset.hpp>
#include <vset/memory/fsb/offset_proxy.hpp>
#include <vset/memory/offset_pointer.hpp>

#include <fas/aop.hpp>

namespace vset { namespace memory{ namespace fsb{

struct aspect_pointer: fas::aspect< typename fas::type_list_n<
  fas::advice<_get_by_offset_, ad_get_by_offset>,
  fas::advice<_offset_by_ptr_, ad_offset_by_ptr>,
  fas::advice<_next_offset_, ad_next_offset>,
  fas::type_advice< _pointer_provider_,  fas::w< offset_pointer< fas::_, offset_proxy<fas::_> > > >
>::type > {};

}}}

#endif
