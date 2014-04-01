//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_INMEM_ASPECT_INMEM_HPP
#define VSET_VSET_BUFFER_INMEM_ASPECT_INMEM_HPP

#include <vset/buffer/inmem/ad_size.hpp>
#include <vset/buffer/inmem/ad_capacity.hpp>
#include <vset/buffer/inmem/ad_resize.hpp>
#include <vset/buffer/inmem/ad_reserve.hpp>
#include <vset/buffer/inmem/ad_clear.hpp>
#include <vset/buffer/inmem/ad_data.hpp>
#include <vset/buffer/inmem/ad_truncate.hpp>
#include <vset/buffer/inmem/tags.hpp>
#include <vset/buffer/tags.hpp>
#include <fas/aop.hpp>
#include <vector>

namespace vset { namespace buffer { namespace inmem{

struct aspect: fas::aspect< fas::type_list_n<
  fas::value< _container_, std::vector<char> >,
  fas::type<_data_type_, char*>,
  fas::type<_size_type_, std::vector<char>::size_type >,
  fas::advice< _data_, ad_data >,
  fas::advice< _size_, ad_size >,
  fas::advice< _capacity_, ad_capacity >,
  fas::advice< _resize_, ad_resize >,
  fas::advice< _clear_, ad_clear >,
  fas::advice< _reserve_, ad_reserve >,
  fas::advice< _truncate_, ad_truncate >
>::type >{};

}}}

#endif
