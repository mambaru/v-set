//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_FIRST_PROPER_NODE_HPP
#define VSET_VTREE_ASPECT_AD_FIRST_PROPER_NODE_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <stdexcept>

namespace vset{ namespace vtree{

struct ad_first_proper_node
{
  template<typename T, typename Itr>
  Itr operator()(T& t, Itr itr )
  {
    typedef typename T::value_type value_type;
    Itr beg = itr;
    Itr end = t.get_container().end();
    
    if (beg==end)
      return itr;
    
    value_type value = itr->first.first;

    for ( ++beg; beg!=end; ++beg )
    {
      if (beg->first.first != value )
        return itr;

      if ( !beg->second->filled() )
        return beg;
    }
    
    return itr;
  }
};
  
}}

#endif
