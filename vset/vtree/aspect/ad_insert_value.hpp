//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_INSERT_VALUE_HPP
#define VSET_VTREE_ASPECT_INSERT_VALUE_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <vset/nullptr.hpp>

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

    const typename T::key_type& key= t.get_aspect().template get<_get_key_>()(t, value);
    container_type& container = t.get_container();
    // находим ближайший подходящий нод
    container_iterator itr = t.get_aspect().template get<_lower_node_>()(t, key);

    if ( itr == container.end() )
    {
      // Сюда попадем если ни одного нода еще не созданно
      itr = t.get_aspect().template get<_create_node_>()(t, value);
      if ( itr == container.end() || itr->second == fas_nullptr)
        return typename T::iterator( container.end(), typename T::iterator::difference_type(0) );
    }
    else
    {
      // Ищем ближайший свободный нод
      itr = t.get_aspect().template get<_first_proper_node_>()(t, itr);
    }

    VSET_NULLPTR_ACCERT(itr->second)

    if ( itr->second->filled() )
    {
      itr = t.get_aspect().template get<_split_node_>()(t, itr, key);
    }

    VSET_NULLPTR_ACCERT(itr->second)

    array_iterator aitr = itr->second->insert(value, t.get_aspect().template get<_value_compare_>() );
    itr = t.get_aspect().template get<_update_node_key_>()(t, itr);

    VSET_NULLPTR_ACCERT(itr->second)

    itr->second->begin();

    ++(t.get_aspect().template get<_size_>());

    return typename T::iterator( itr, std::distance(itr->second->begin(), aitr) );
  }
};

}}

#endif
