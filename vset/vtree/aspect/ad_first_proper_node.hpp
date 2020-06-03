//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_FIRST_PROPER_NODE_HPP
#define VSET_VTREE_ASPECT_AD_FIRST_PROPER_NODE_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <vset/nullptr.hpp>

namespace vset{ namespace vtree{

struct ad_first_proper_node
{
  template<typename T, typename V>
  bool not_equal(T& t, const V& first, const V& second) const
  {
    return t.get_aspect().template get<_key_compare_>()(first, second)
      || t.get_aspect().template get<_key_compare_>()(second, first);
  }

  template<typename T, typename Itr>
  Itr operator()(T& t, Itr itr ) const
  {
    typedef typename T::key_type key_type;
    Itr beg = itr;
    Itr end = t.get_container().end();

    const key_type& value = itr->first.first;
    for ( ++beg; beg != end; ++beg )
    {
      if ( not_equal(t, beg->first.first, value) )
      {
        return itr;
      }

      VSET_NULLPTR_ACCERT(beg->second);
      if ( !beg->second->filled() )
      {
        return beg;
      }
    }

    return itr;
  }
};

}}

#endif
