//
// Author: Saprykin Dmitry <saprykin.dmitry@gmail.com>, (C) 2014
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_SWAP_CONTAINER_HPP
#define VSET_VTREE_ASPECT_AD_SWAP_CONTAINER_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <string.h>
#include <stdlib.h>

namespace vset{ namespace vtree{

struct ad_swap_container
{
  template<typename T>
  void operator()(T& a, T& b)
  {
    a.get_container().swap(b.get_container());
    std::swap(a.get_aspect().template get<_size_>(), b.get_aspect().template get<_size_>());
  }
};

}}

#endif
