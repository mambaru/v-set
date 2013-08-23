//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMMORY_STRATEGY_HPP
#define VSET_VSET_MEMMORY_STRATEGY_HPP

#include <vset/memory/fsb/aspect.hpp>
#include <vset/buffer/persistent/mmap/aspect.hpp>
#include <vset/buffer/persistent/filesync/aspect.hpp>
#include <vset/buffer/inmem/aspect.hpp>

#include <vset/buffer/persistent_provider.hpp>
#include <vset/buffer/provider.hpp>

namespace vset { namespace memory{

namespace strategy
{
  template<typename T>
  struct fsb_mmap
    : fsb::aspect<
        T,
        vset::buffer::persistent::mmap::aspect,
        buffer::persistent_provider
      >
  {};

  template<typename T>
  struct fsb_filesync
    : fsb::aspect<
        T,
        vset::buffer::persistent::filesync::aspect,
        buffer::persistent_provider
      >
  {};

  template<typename T>
  struct fsb_inmem
    : fsb::aspect<
        T,
        vset::buffer::inmem::aspect,
        buffer::provider
      >
  {};
}
  
}}

#endif
