#ifndef VSET_MULTI_SET_HPP
#define VSET_MULTI_SET_HPP

#include <fas/typemanip/has_typename.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>

namespace vset{ 

template<typename V, typename C = std::less<V>, typename A = std::allocator<V> >
class multiset;
  
template<typename V, typename C>
class multiset<V, C, std::allocator<V> >
  : public vtree::vtree< vtree::strategy::vtree_std_alloc<V, C, 512> >
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
  struct multiset;

  template<typename V, typename C, typename A>
  struct multiset<V, C, A, true>
    // : public vtree::vtree< typename multiset_helper<V, C, A>::aspect  >
  {
    typedef vtree::vtree< typename multiset_helper<V, C, A>::aspect  > type;
    /*
    typedef multiset<V, C, A, true> self;
    typedef vtree::vtree< typename multiset_helper<V, C, A>::aspect  > super;
    */
  };
}

template<typename V, typename C, typename A >
class multiset
  : public helper::multiset<V, C, A, helper::is_aspect_maker<A>::value >::type
{
  typedef typename helper::multiset<V, C, A, helper::is_aspect_maker<A>::value >::type super;
  
public:
  typedef typename super::key_type    key_type;
  typedef typename super::value_type  value_type;
  typedef typename super::key_compare    key_compare;
  typedef typename super::value_compare  value_compare;
  typedef typename super::allocator_type allocator_type;
  typedef typename super::reference reference;
  typedef typename super::const_reference const_reference;
  typedef typename super::pointer pointer;
  typedef typename super::const_pointer const_pointer;
  typedef typename super::iterator iterator;
  typedef typename super::const_iterator const_iterator;
  typedef typename super::reverse_iterator reverse_iterator;
  typedef typename super::const_reverse_iterator const_reverse_iterator;
  typedef typename super::difference_type difference_type;
  typedef typename super::size_type size_type;
  
  multiset(): super() {}

  explicit multiset(const key_compare& comp,const allocator_type& alloc = allocator_type() )
    : super(comp, alloc)
  {
  }

  template<typename InputIterator>
  multiset(InputIterator beg, InputIterator end)
    : super( beg,  end)
  {
  }

  template<typename InputIterator>
  multiset(InputIterator beg, InputIterator end, const value_compare& comp, const allocator_type&  alloc= allocator_type() )
    : super( beg,  end, comp, alloc)
  {
  }

  multiset(const multiset& other)
    : super( other )
  {
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  multiset(multiset&& other)
    : super( other )
  {
  }

  multiset( std::initializer_list<value_type> il, const value_compare& comp= value_compare(), const allocator_type&  alloc= allocator_type())
    : super( il, comp, alloc )
  {
  }

#endif

  multiset&  operator=(const multiset& other)
  {
    super::operator = (other);
    return *this;
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  multiset& operator=(multiset&& other)
  {
    super::operator = (other);
    return *this;
  }

  multiset& operator=( std::initializer_list<value_type> il)
  {
    super::operator = (il);
    return *this;
  }
#endif

  
  
};


}

#endif
