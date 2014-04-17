//
// Author: Dmitry Saprykin <saprykin.dmitry@gmail.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_AD_DEFRAG_CONTAINER_HPP
#define VSET_VTREE_ASPECT_AD_DEFRAG_CONTAINER_HPP

#include <vset/vtree/aspect/tags.hpp>

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

    container_type& container = t.get_container();
    //container_iterator itr = container.erase( block_itr.get_source_iterator(), block_itr.get_source_iterator() );
    container_iterator itr = block_itr.get_source_iterator();
    if( itr != container.end() ) {
      container_iterator next = itr;
      next++;
      if( next != container.end() ) {
        if( 2 * (itr->second->size() + next->second->size()) <= itr->second->capacity() ) {
          for(unsigned int i = 0; i < next->second->size(); i++) {
            itr->second->push_back(next->second->at(i), t.get_aspect().template get<_compare_>());
          }
          container.erase( next );
          t.get_allocator().deallocate(next->second, 1);
          t.get_aspect().template get<_update_node_key_>()(t, itr);
        }
      }
    }
  }
};
  
}}

#endif
