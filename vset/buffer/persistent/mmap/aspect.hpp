//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_MMAP_ASPECT_MMAP_HPP
#define VSET_VSET_BUFFER_PERSISTENT_MMAP_ASPECT_MMAP_HPP


#include <vset/buffer/persistent/file/aspect.hpp>
#include <vset/buffer/persistent/mmap/aspect_basic.hpp>
#include <fas/aop.hpp>


namespace vset { namespace buffer{ namespace persistent{ namespace mmap{

struct aspect: fas::aspect_merge<
  file::aspect,
  aspect_basic
>::type {};

}}}}

#endif

