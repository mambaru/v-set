//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_ERASE_ITERATOR_HPP
#define VSET_VTREE_ASPECT_AD_ERASE_ITERATOR_HPP

#include <vset/vtree/aspect/tags.hpp>

namespace vset{ namespace vtree{

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  
struct ad_erase_iterator
{
  template<typename T>
  typename T::const_iterator
  operator()(T& t, typename T::const_iterator itr)
  {
    throw;
    /*
    if ( itr == t.cend() )
      return itr;

    // НЕ ВЕРНО!!! итератор не валидный!
    typename T::const_iterator result = itr + 1;
    
    itr.get_source_iteartor()->second->erase(
      itr.get_source_iteartor()->second->begin()
      + itr.get_position()
    );

    if ( itr.get_source_iteartor()->second->empty() )
    {
      t.get_container().erase(itr.get_source_iteartor());
    }
    else
    {
      t.get_aspect().template get<_update_node_key_>()(t, itr.get_source_iteartor() );
    }
    
    --t.get_aspect().template get<_size_>();
    */
    //return result;
  }
};

#else

struct ad_erase_iterator
{
  template<typename T>
  typename T::iterator operator()(T& t, typename T::iterator itr)
  {
    if ( itr == t.end() )
      throw std::out_of_range("ad_erase_iterator");
      //return itr;

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
