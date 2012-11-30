//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_NODE_FOR_INSERT_HPP
#define VSET_VTREE_ASPECT_AD_NODE_FOR_INSERT_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <stdexcept>

namespace vset{ namespace vtree{

struct ad_node_for_insert
{
  template<typename T, typename Itr, typename V>
  Itr operator()(T& , Itr itr1, Itr itr2, const V& value )
  {
    if ( value <= itr1->first.first )
      return itr1;

    if ( value >= itr2->first.first )
      return itr2;
    
    if ( itr1->first.second <= value
         && itr2->first.first >= value )
    {
      // Можно вставить в любой, выбираем меньший
      return itr1->second->size() < itr2->second->size()
             ? itr1
             : itr2;
    }

    
    if ( itr1->first.first >= value && itr1->first.second <= value )
      return itr1;

    if ( itr2->first.first >= value && itr2->first.second <= value )
      return itr2;
    

    std::cout << std::endl;
    std::cout << value << std::endl;
    std::cout << itr1->first.first << std::endl;
    std::cout << itr1->first.second << std::endl;
    std::cout << itr2->first.first << std::endl;
    std::cout << itr2->first.second << std::endl;
    
    throw std::logic_error("ad_node_for_insert");

  }
  
};

  
}}

#endif
