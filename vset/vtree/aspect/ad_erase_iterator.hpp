//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_ERASE_ITERATOR_HPP
#define VSET_VTREE_ASPECT_AD_ERASE_ITERATOR_HPP

#include <vset/vtree/aspect/tags.hpp>

#include <iostream>

namespace vset{ namespace vtree{

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  
struct ad_erase_iterator
{
  template<typename T>
  typename T::const_iterator
  operator()(T& t, typename T::const_iterator itr)
  {
    if ( itr == t.end() )
      throw std::out_of_range("ad_erase_iterator itr == t.end()");

    typedef typename T::iterator iterator;
    typedef typename iterator::difference_type difference_type;
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator container_iterator;


    iterator itr2 = t.begin() + std::distance(t.cbegin(), itr); /*itr.get_position()*/;
    
    difference_type    offset   = itr2.get_position();
    container_iterator cont_itr = itr2.get_source_iteartor();


    cont_itr->second->erase(
      cont_itr->second->cbegin() + offset,
      t.get_aspect().template get<_compare_>()                        
    );

    if ( cont_itr->second->empty() )
    {
      t.get_allocator().deallocate(cont_itr->second, 1);
      t.get_container().erase( cont_itr++ );
      offset = 0;
    }
    else
    {
      cont_itr = t.get_aspect().template get<_update_node_key_>()(t, cont_itr );
    }

    --t.get_aspect().template get<_size_>();

    return iterator(cont_itr, offset);
    
    
    throw std::logic_error("not impl");
  }
};

#else

struct ad_erase_iterator
{
  template<typename T>
  typename T::iterator operator()(T& t, typename T::iterator itr)
  {
    if ( itr == t.end() )
      throw std::out_of_range("ad_erase_iterator itr == t.end()");

    typedef typename T::iterator iterator;
    typedef typename iterator::difference_type difference_type;
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator container_iterator;

    container_iterator cont_itr = itr.get_source_iteartor();
    difference_type    offset   = itr.get_position();
    

    cont_itr->second->erase(
      cont_itr->second->begin()
      + itr.get_position()
    );

    if ( cont_itr->second->empty() )
    {
      t.get_allocator().deallocate(cont_itr->second, 1);
      t.get_container().erase( cont_itr++ );
      offset = 0;
    }
    else
    {
      cont_itr = t.get_aspect().template get<_update_node_key_>()(t, cont_itr );
    }

    --t.get_aspect().template get<_size_>();

    return iterator(cont_itr, offset);
  }
};

#endif

}}

#endif
