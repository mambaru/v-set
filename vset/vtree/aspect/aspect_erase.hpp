//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_ASPECT_ERASE_HPP
#define VSET_VTREE_ASPECT_ASPECT_ERASE_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <vset/vtree/aspect/ad_clear.hpp>
#include <vset/vtree/aspect/ad_erase_value.hpp>
#include <vset/vtree/aspect/ad_erase_range.hpp>
#include <vset/vtree/aspect/ad_erase_iterator.hpp>
#include <fas/aop.hpp>
#include <fas/type_list.hpp>

namespace vset{ namespace vtree{

struct aspect_erase: fas::aspect< fas::type_list_n<
  fas::advice< _clear_, ad_clear>,
  fas::advice< _erase_iterator_, ad_erase_iterator>,
  fas::advice< _erase_range_, ad_erase_range>,
  fas::advice< _erase_value_, ad_erase_value>
>::type > {};

}}

#endif
