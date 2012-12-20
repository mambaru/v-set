//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_INSERT_VALUE_HPP
#define VSET_VTREE_ASPECT_INSERT_VALUE_HPP

#include <vset/vtree/aspect/tags.hpp>

namespace vset{ namespace vtree{

struct ad_insert_value
{
  template<typename T>
  typename T::iterator
  operator()(T& t, const typename T::value_type& value)
  {
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator container_iterator;
    typedef typename T::allocator_type allocator_type;
    typedef typename allocator_type::value_type array_type;
    typedef typename array_type::iterator array_iterator;

    container_type& container = t.get_container();
    // находим ближайший подходящий нод
    container_iterator itr = t.get_aspect().template get<_lower_node_>()(t, value);
    if ( itr == container.end() )
    {
      // Сюда попадем если ниодного нода еще не созданно
      itr = t.get_aspect().template get<_create_node_>()(t, value);
    }
    else
    {
      // Ищем ближайший свободный но
      itr = t.get_aspect().template get<_first_proper_node_>()(t, itr);
    }

    if ( itr == container.end() )
      abort();

    if ( itr->second->filled() )
      itr = t.get_aspect().template get<_split_node_>()(t, itr, value);

    if ( itr == container.end() )
      abort();
    itr->second->begin();
    
    array_iterator aitr = itr->second->insert(value, t.get_aspect().template get<_compare_>() );
    itr = t.get_aspect().template get<_update_node_key_>()(t, itr);
    if ( itr == container.end() )
      abort();
    itr->second->begin();
    
    ++(t.get_aspect().template get<_size_>());
    
    return typename T::iterator( itr, std::distance(itr->second->begin(), aitr) );
  }
};

  
}}

#endif
