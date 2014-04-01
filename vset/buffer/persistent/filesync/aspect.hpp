//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_FILESYNC_ASPECT_HPP
#define VSET_VSET_BUFFER_PERSISTENT_FILESYNC_ASPECT_HPP

#include <vset/buffer/persistent/file/aspect.hpp>
#include <vset/buffer/persistent/filesync/aspect_basic.hpp>
#include <fas/aop.hpp>


namespace vset { namespace buffer{ namespace persistent{ namespace filesync{

struct aspect: fas::merge_aspect<
  file::aspect,
  aspect_basic
>::type {};

}}}}

#endif

