//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_HPP

#include <vset/memory/tags.hpp>
#include <vset/memory/fsb/tags.hpp>
#include <vset/memory/fsb/aspect/aspect_value.hpp>
#include <vset/memory/fsb/aspect/aspect_manager.hpp>
#include <vset/memory/fsb/aspect/aspect_pointer.hpp>
#include <vset/buffer/persistent/mmap/aspect.hpp>
#include <fas/aop.hpp>

namespace vset { namespace memory{ namespace fsb{

template<typename T, typename BufferAspect = vset::buffer::persistent::mmap::aspect >
struct aspect: fas::aspect_merge<
  aspect_value<T>,
  aspect_manager<>,
  aspect_pointer,
  BufferAspect
>::type {};

}}}

#endif
