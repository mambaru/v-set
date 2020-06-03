//
// Author: Dmitry Saprykin <saprykin.dmitry@gmail.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_DEFRAG_CONTAINER_HPP
#define VSET_VTREE_ASPECT_AD_DEFRAG_CONTAINER_HPP

#include <vset/vtree/aspect/tags.hpp>
#include <vset/nullptr.hpp>

namespace vset{ namespace vtree{

struct ad_defrag_container
{
  template<typename T>
  struct helper
  {
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator iterator;
  };

  template<typename T>
  void operator()(T& t, typename T::iterator block_itr)
  {
    typedef typename helper<T>::container_type container_type;
    typedef typename container_type::iterator container_iterator;
    typedef typename helper<T>::iterator iterator;
    typedef typename iterator::value_type::second_type array_pointer;

    container_type& container = t.get_container();
    container_iterator itr = block_itr.get_source_iterator();
    if( itr != container.end() )
    {
      container_iterator next = itr;
      array_pointer first_arr = next->second;
      VSET_NULLPTR_ACCERT(first_arr)
      next++;
      if( next != container.end() )
      {
        array_pointer next_arr = next->second;
        VSET_NULLPTR_ACCERT(next_arr)
        if( 2 * (first_arr->size() + next_arr->size()) <= first_arr->capacity() )
        {
          for(unsigned int i = 0; i < next_arr->size(); i++)
          {
            itr->second->push_back(next_arr->at(i), t.get_aspect().template get<_compare_>());
          }
          container.erase( next );
          t.get_allocator().deallocate(next_arr, 1);
          t.get_aspect().template get<_update_node_key_>()(t, itr);
        }
      }
    }
  }
};

}}

#endif
