//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_FILESYNC_ASPECT_FILESYNC_HPP
#define VSET_VSET_BUFFER_PERSISTENT_FILESYNC_ASPECT_FILESYNC_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>
#include <vset/buffer/persistent/filesync/tags.hpp>
#include <vset/buffer/persistent/filesync/ad_open.hpp>
#include <vset/buffer/persistent/filesync/ad_clear.hpp>
#include <vset/buffer/persistent/filesync/ad_close.hpp>
#include <vset/buffer/persistent/filesync/ad_reserve.hpp>
#include <vset/buffer/persistent/filesync/ad_resize.hpp>
#include <vset/buffer/persistent/filesync/ad_sync.hpp>
#include <vset/buffer/persistent/filesync/ad_truncate.hpp>
#include <fas/aop.hpp>


namespace vset { namespace buffer{ namespace persistent{ namespace filesync{

struct aspect_filesync: fas::aspect< fas::type_list_n<
  fas::group<_destructor_, _close_>,
  fas::value_advice<_buffer_size_, size_t>,
  fas::advice<_open_, ad_open>,
  fas::advice<_close_, ad_close>,
  fas::advice<_reserve_, ad_reserve>,
  fas::advice<_resize_, ad_resize>,
  fas::advice<_truncate_, ad_truncate>,
  fas::advice<_clear_, ad_clear>,
  fas::advice<_sync_, ad_sync>
>::type >{};

}}}}

#endif

