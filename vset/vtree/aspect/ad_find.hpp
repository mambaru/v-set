//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_FIND_HPP
#define VSET_VTREE_ASPECT_AD_FIND_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <algorithm>
#include <iostream>

namespace vset{ namespace vtree{

struct ad_find
{
  template<typename T>
  typename T::iterator
  operator()(T& t, const typename T::key_type& value)
  {    typename T::iterator itr = t.get_aspect().template get<_lower_bound_>()(t, value);

    if ( itr == t.end() )
    {
      return t.end();
    }

    if ( t.get_aspect().template get<_key_compare_>()(*itr, value)
       || t.get_aspect().template get<_key_compare_>()(value, *itr) )
    {
      return t.end();
    }

    return itr;
  }
};

}}

#endif
