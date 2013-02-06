#ifndef VSET_MULTI_SET_HPP
#define VSET_MULTI_SET_HPP

#include <fas/typemanip/has_typename.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/aspect/aspect.hpp>

namespace vset{ 

template<typename V, typename C = std::less<V>, typename A = std::allocator<V> >
class multiset;
  
template<typename V, typename C>
class multiset<V, C, std::allocator<V> >
  : public vtree::vtree< vtree::aspect2<V, C, 512> >
{
  
};

namespace helper
{
  FAS_HAS_TYPENAME(is_aspect_maker, aspect_maker)
  
  template<typename V, typename C, typename A>
  struct multiset_helper
  {
    typedef typename A::template apply<V, C>::type aspect;
  };

  template<typename V, typename C, typename A, bool >
  class multiset;

  template<typename V, typename C, typename A>
  class multiset<V, C, A, true>
    : public vtree::vtree< typename multiset_helper<V, C, A>::aspect  >
  {
    typedef multiset<V, C, A, true> self;
    typedef vtree::vtree< typename multiset_helper<V, C, A>::aspect  > super;
  };
}

template<typename V, typename C, typename A >
class multiset
  : public helper::multiset<V, C, A, helper::is_aspect_maker<A>::value >
{
  
};


}

#endif
