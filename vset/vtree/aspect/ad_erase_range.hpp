//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_ERASE_RANGE_HPP
#define VSET_VTREE_ASPECT_AD_ERASE_RANGE_HPP

#include <vset/vtree/aspect/tags.hpp>

namespace vset{ namespace vtree{


#ifdef __GXX_EXPERIMENTAL_CXX0X__

struct ad_erase_range
{
  template<typename T>
  typename T::iterator
  operator()(T& t, typename T::const_iterator beg, typename T::const_iterator end)
  {
    typename T::iterator last = t.end();
    size_t dist = std::distance(beg, end);
    if (dist!=0)
    {
      last = t.get_aspect().template get<_erase_iterator_>()(t, beg);
      for ( --dist;dist!=0; --dist)
        last = t.get_aspect().template get<_erase_iterator_>()(t, last);
    }
    
    return last;
  }
};

#else

struct ad_erase_range
{
  template<typename T>
  void operator()(T& t, typename T::iterator beg, typename T::iterator end)
  {
    //for ( ;beg!=end;)
    size_t dist = std::distance(beg, end);
    for ( ;dist!=0; --dist)
      beg = t.get_aspect().template get<_erase_iterator_>()(t, beg);
  }
};

#endif

}}

#endif
