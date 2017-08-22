//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMMORY_STRATEGY_HPP
#define VSET_VSET_MEMMORY_STRATEGY_HPP

#include <vset/memory/fsb/aspect.hpp>
#include <vset/memory/fsb/aspect/aspect_pointer.hpp>
#include <vset/buffer/persistent/mmap/aspect.hpp>
#include <vset/buffer/persistent/filesync/aspect.hpp>
#include <vset/buffer/inmem/aspect.hpp>
#include <vset/buffer/persistent_provider.hpp>
#include <vset/buffer/provider.hpp>

namespace vset { namespace memory{

namespace strategy
{
  /*typedef fsb::aspect_offset    fsb_offset;
  typedef fsb::aspect_nth  fsb_nth;*/

  template<typename T, typename OffsetAspect>
  struct fsb_mmap
    : fsb::aspect<
        T,
        OffsetAspect,
        vset::buffer::persistent::mmap::aspect,
        buffer::persistent_provider
      >
  {};

  template<typename T, typename OffsetAspect>
  struct fsb_filesync
    : fsb::aspect<
        T,
        OffsetAspect,
        vset::buffer::persistent::filesync::aspect,
        buffer::persistent_provider
      >
  {};

  template<typename T, typename OffsetAspect>
  struct fsb_inmem
    : fsb::aspect<
        T,
        OffsetAspect,
        vset::buffer::inmem::aspect,
        buffer::provider
      >
  {};
}
  
}}

#endif
