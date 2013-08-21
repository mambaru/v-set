//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_STRATEGY_HPP
#define VSET_VSET_BUFFER_STRATEGY_HPP

#include <vset/buffer/persistent/filesync/aspect.hpp>
#include <vset/buffer/persistent/mmap/aspect.hpp>
#include <vset/buffer/inmem/aspect.hpp>

namespace vset { namespace buffer{

namespace strategy
{
  typedef ::vset::buffer::inmem::aspect inmem;
  typedef ::vset::buffer::persistent::filesync::aspect filesync;
  typedef ::vset::buffer::persistent::mmap::aspect mmap;
}
  
}}

#endif
