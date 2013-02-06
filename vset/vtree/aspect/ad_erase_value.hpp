//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_ERASE_VALUE_HPP
#define VSET_VTREE_ASPECT_AD_ERASE_VALUE_HPP

#include <vset/vtree/aspect/tags.hpp>

namespace vset{ namespace vtree{

#ifdef __GXX_EXPERIMENTAL_CXX0X__

struct ad_erase_value
{
  template<typename T>
  typename T::size_type operator()(T& t, const typename T::value_type& value)
  {
    typename T::const_iterator lower = t.get_aspect().template get<_lower_bound_>()(t, value);
    typename T::const_iterator upper = t.get_aspect().template get<_upper_bound_>()(t, value);
    typename T::size_type result = std::distance(lower, upper);
    
    t.get_aspect().template get<_erase_range_>()
    (
      t,
      lower,
      upper
    );
    return result;
  }
};

#else

struct ad_erase_value
{
  template<typename T>
  typename T::size_type operator()(T& t, const typename T::value_type& value)
  {
    typename T::iterator lower = t.get_aspect().template get<_lower_bound_>()(t, value);
    typename T::iterator upper = t.get_aspect().template get<_upper_bound_>()(t, value);
    typename T::size_type result = std::distance(lower, upper);

    t.get_aspect().template get<_erase_range_>()
    (
      t,
      lower,
      upper
    );
    return result;
  }
};


#endif
  

  
}}

#endif
