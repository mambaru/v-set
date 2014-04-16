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
#include <fas/aop.hpp>

namespace vset { namespace memory{ namespace fsb{

template<
  typename T,
  typename BufferAspect,
  template<typename> class BufferProvider
>
struct aspect: fas::merge_aspect<
  aspect_value<T>,
  aspect_manager<BufferProvider>,
  aspect_pointer,
  BufferAspect
>::type {};

}}}

#endif
