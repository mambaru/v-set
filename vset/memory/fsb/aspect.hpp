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

#include <vset/buffer/persistent/filesync/aspect_filesync.hpp>
#include <vset/buffer/persistent/basic/aspect_basic.hpp>

#include <fas/aop.hpp>

namespace vset { namespace memory{ namespace fsb{

template<typename T>
struct aspect: fas::aspect_merge<
  aspect_value<T>,
  aspect_manager,
  aspect_pointer,
  vset::buffer::persistent::filesync::aspect_filesync,
  vset::buffer::persistent::aspect_basic
>::type {};

}}}

#endif
