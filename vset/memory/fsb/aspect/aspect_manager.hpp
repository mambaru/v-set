//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_ASPECT_MANAGER_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_ASPECT_MANAGER_HPP

#include <vset/memory/tags.hpp>
#include <vset/memory/fsb/tags.hpp>
#include <vset/memory/fsb/aspect/ad_begin.hpp>
#include <vset/memory/fsb/aspect/ad_end.hpp>
#include <vset/memory/fsb/aspect/ad_acquire.hpp>
#include <vset/memory/fsb/aspect/ad_allocate.hpp>
#include <vset/memory/fsb/aspect/ad_deallocate.hpp>

#include <vset/buffer/persistent_provider.hpp>

#include <fas/aop.hpp>

namespace vset { namespace memory{ namespace fsb{


struct aspect_manager: fas::aspect< typename fas::type_list_n<
  fas::advice<_begin_, ad_begin>,
  fas::advice<_end_, ad_end>,
  fas::advice<_acquire_, ad_acquire>,
  fas::advice<_allocate_, ad_allocate>,
  fas::advice<_deallocate_, ad_deallocate>,
  fas::type_advice< _buffer_provider_,  fas::w< buffer::persistent_provider< fas::_> > >
>::type > {};

}}}

#endif
