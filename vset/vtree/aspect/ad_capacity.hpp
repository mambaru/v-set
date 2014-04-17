//
// Author: Dmitry Saprykin <saprykin.dmitry@gmail.com>, (C) 2014
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_CAPACITY_HPP
#define VSET_VTREE_ASPECT_AD_CAPACITY_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <stdlib.h>

namespace vset{ namespace vtree{

struct ad_capacity
{
  template<typename T>
  size_t operator()(const T& t) const
  {
    return t.get_container().size() * T::aspect::template advice_cast< ::vset::vtree::_array_type_ >::type::dimension;
  }
};

}}

#endif
