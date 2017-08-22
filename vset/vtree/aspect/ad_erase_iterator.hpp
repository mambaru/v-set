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
  typename T::iterator
  operator()(T& t, typename T::const_iterator itr)
  {
    return this->operator ()(t, itr, true);
  }

  template<typename T>
  typename T::iterator
  operator()(T& t, typename T::const_iterator itr, bool make_defrag) const 
  {
    typedef typename T::iterator iterator;
    typedef typename iterator::difference_type difference_type;
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator container_iterator;

    if ( itr == t.end() )
      return iterator(t.get_container().end(), 0 );

    difference_type    offset   = itr.get_position();
    container_iterator cont_itr = t.get_container().erase( itr.get_source_iterator(), itr.get_source_iterator() );
    if ( cont_itr == t.get_container().end() )
      return iterator( cont_itr, 0 );
    
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
      if ( offset == static_cast<difference_type>(cont_itr->second->size()) )
      {
        offset=0;
        ++cont_itr;
      }
    }

    --t.get_aspect().template get<_size_>();

    if( make_defrag )
    {
      t.get_aspect().template get<_defrag_container_>()(t, iterator(cont_itr, offset));
    }

    return iterator(cont_itr, offset);
  }
};

#else

struct ad_erase_iterator
{
  template<typename T>
  typename T::iterator operator()(T& t, typename T::iterator itr)
  {
    return this->operator ()(t, itr, true);
  }

  template<typename T>
  typename T::iterator operator()(T& t, typename T::iterator itr, bool make_defrag)
  {
    if ( itr == t.end() )
    {
      throw std::out_of_range("ad_erase_iterator itr == t.end()");
    }

    typedef typename T::iterator iterator;
    typedef typename iterator::difference_type difference_type;
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator container_iterator;

    container_iterator cont_itr = itr.get_source_iterator();
    difference_type    offset   = itr.get_position();
    

    cont_itr->second->erase(
      cont_itr->second->begin() + itr.get_position(),
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
      if ( offset == static_cast<difference_type>(cont_itr->second->size()) )
      {
        offset=0;
        ++cont_itr;
      }
    }

    --t.get_aspect().template get<_size_>();

    if( make_defrag )
    {
      t.get_aspect().template get<_defrag_container_>()(t, iterator(cont_itr, offset));
    }

    return iterator(cont_itr, offset);
  }
};

#endif

}}

#endif
