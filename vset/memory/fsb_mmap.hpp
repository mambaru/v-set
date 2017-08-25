//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2017
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMMORY_FSB_MMAP_HPP
#define VSET_VSET_MEMMORY_FSB_MMAP_HPP

#include <vset/memory/strategy.hpp>
#include <vset/memory/manager.hpp>
#include <fas/aop.hpp>

namespace vset { namespace memory{

template<typename T, typename OffsetAspect = fsb_offset, typename A = fas::aspect<> >
struct fsb_mmap
  : manager< typename fas::merge_aspect< A, strategy::fsb_mmap<T, fsb_offset> >::type >
{};

}}

#endif