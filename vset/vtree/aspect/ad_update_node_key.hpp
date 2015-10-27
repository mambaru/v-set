//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_UPDATE_NODE_KEY_HPP
#define VSET_VTREE_ASPECT_AD_UPDATE_NODE_KEY_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <string.h>
#include <stdlib.h>

namespace vset{ namespace vtree{

struct ad_update_node_key
{
  template<typename T, typename Iterator>
  Iterator operator()(T& t, Iterator itr )
  {
    typedef Iterator iterator;
    typedef typename T::key_type key_type;
    typedef typename iterator::value_type::second_type array_pointer;
    typedef typename T::aspect::template advice_cast<_key_compare_>::type compare_type;
    compare_type& compare = t.get_aspect().template get<_key_compare_>();
    array_pointer arr = itr->second;
    
    if ( !(!compare(arr->front(), itr->first.first)    && !compare(itr->first.first, arr->front()) )
         || !(!compare(arr->back(),  itr->first.second)   && !compare(itr->first.second, arr->back()) )
       )
    {
      t.get_container().erase(itr);
      
      const typename T::aspect::template advice_cast<_get_key_>::type& get_key = t.get_aspect().template get<_get_key_>();
      const key_type& k1f = get_key(t, arr->front());
      const key_type& k1b = get_key(t, arr->back());

      itr = t.get_aspect().template get<_insert_to_container_>()(t, std::make_pair( /*arr->front(), arr->back()*/k1f, k1b ), arr );
    }

    return itr;
  }
};

}}

#endif
