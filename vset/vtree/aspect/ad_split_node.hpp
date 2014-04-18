//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_SPLIT_NODE_HPP
#define VSET_VTREE_ASPECT_AD_SPLIT_NODE_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <stdlib.h>

namespace vset{ namespace vtree{

struct ad_split_node
{
  template<typename T>
  struct helper
  {
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator iterator;
  };

  template<typename T>
  typename helper<T>::iterator
  operator()(T& t, typename helper<T>::iterator itr, const typename T::value_type& value)
  {
    typedef typename T::value_type value_type;
    typedef typename helper<T>::iterator container_iterator;
    typedef typename helper<T>::container_type container_type;

    typedef typename T::allocator_type allocator_type;
    typedef typename allocator_type::pointer array_pointer;

    container_type& container = t.get_container();
    if ( itr == container.end() )
    {
      return itr;
    }

    array_pointer arr1 = itr->second;
    array_pointer arr2 = t.get_allocator().allocate(1);
    if ( !arr2 )
    {
      abort();//return container.end(); // TODO: проверить что offset_pointer при сравнении с числом не сравнивает оффыеты
    }

    size_t offset = arr1->size();

    if ( offset < 2 )
    {
      return itr;
    }

    offset /= 2;

    arr2->assign( arr1->begin() + offset, arr1->end(), t.get_aspect().template get<_compare_>() );
    arr1->resize( offset, value_type(), t.get_aspect().template get<_compare_>() );
    container.erase( itr );
    container_iterator itr1 = t.get_aspect().template get<_insert_to_container_>()(t, std::make_pair(arr1->front(), arr1->back()), arr1);
    container_iterator itr2 = t.get_aspect().template get<_insert_to_container_>()(t, std::make_pair(arr2->front(), arr2->back()), arr2);

    itr = t.get_aspect().template get<_node_for_insert_>()(t, itr1, itr2, value);

    arr1 = itr->second;
    return itr;
  }
};

}}

#endif
