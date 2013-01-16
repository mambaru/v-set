#ifndef VSET_VTREE_VTREE_HPP
#define VSET_VTREE_VTREE_HPP

#include <cstddef>
#include <exception>
#include <map>

#include <vset/vtree/vtree_iterator.hpp>
#include <vset/vtree/aspect/aspect.hpp>
/*#include <functional>
#include <memory>
#include <map>
*/

namespace vset{ namespace vtree{

struct not_impl: std::domain_error
{
  not_impl(): std::domain_error("not_impl") {}
  not_impl(const char* what): std::domain_error(what) {}
};



template< typename A = fas::aspect<> >
class vtree:
  public fas::aspect_class<A>
{
  typedef vtree<A> self;
  typedef fas::aspect_class<A> super;
  typedef typename super::aspect::template advice_cast<_allocator_>::type         allocator_builder;
  typedef typename super::aspect::template advice_cast<_key_compare_>::type       key_compare;
  typedef typename super::aspect::template advice_cast<_value_compare_>::type     value_compare;

public:
  typedef typename super::aspect::template advice_cast<_value_type_>::type        value_type;
  typedef typename super::aspect::template advice_cast<_key_type_>::type          key_type;

  //typedef typename super::aspect::template advice_cast<_container_>::type         container_type;
  typedef typename allocator_builder::template apply<self>::type                  allocator_type;


  //typedef typename key_compare_builder::template build<self>::type                key_compare;
  //typedef typename value_compare_builder::template build<self>::type              value_compare;




  /*
  typedef int key_type;
  typedef int value_type;
  typedef std::less<int> key_compare;
  typedef std::less<int> value_compare;

  typedef _Key     key_type;
  typedef _Key     value_type;
  typedef _Compare key_compare;
  typedef _Compare value_compare;
  typedef _Alloc   allocator_type;

  */
  // Ключ - пара значений [первый элемент массива, последний элемент массива]
  /*
  typedef typename super::aspect::template advice_cast<_key_type_>::type   key_type;
  typedef typename super::aspect::template advice_cast<_value_type_>::type value_type;

  typedef typename super::aspect::template advice_cast<_key_compare_>::type   key_compare;
  typedef typename super::aspect::template advice_cast<_value_compare_>::type value_compare;

  typedef typename super::aspect::template advice_cast<_allocator_type_>::type allocator_type;

  typedef typename allocator_type::pointer             pointer;
  typedef typename allocator_type::const_pointer       const_pointer;
  typedef typename allocator_type::reference           reference;
  typedef typename allocator_type::const_reference     const_reference;
  typedef typename allocator_type::size_type           size_type;
  typedef typename allocator_type::difference_type     difference_type;
  */

  /*
  typedef std::multimap< key_type,  value_type, key_compare > conatainer_type;
  typedef typename conatainer_type::allocator_type allocator_type;
  typedef typename allocator_type::pointer             pointer;
  typedef typename allocator_type::const_pointer       const_pointer;
  typedef typename allocator_type::reference           reference;
  typedef typename allocator_type::const_reference     const_reference;
  */
  typedef typename allocator_type::size_type           size_type;
  typedef typename allocator_type::difference_type     difference_type;
  /*typedef typename container_type::iterator container_iterator;
  typedef typename container_type::const_iterator const_iterator;
  typedef typename container_type::reverse_iterator reverse_iterator;
  typedef typename container_type::const_reverse_iterator const_reverse_iterator;
  */

  typedef std::multimap< std::pair<value_type, value_type>, typename allocator_type::pointer> container_type;

  typedef vtree_iterator<typename container_type::iterator, value_type> iterator;
  typedef vtree_iterator<typename container_type::const_iterator, const value_type> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;




  // key_compare     _comparator;
  allocator_type  _allocator;
  container_type  _container;

public:

  vtree()
    : _allocator( this->get_aspect().template get<_allocator_>()(*this) )
  {}

  explicit vtree(const key_compare& comp,const allocator_type& alloc = allocator_type() )
    : _allocator(alloc)
  {
    this->get_aspect().template get<_compare_>() = comp;
  }

  template<typename InputIterator>
  vtree(InputIterator beg, InputIterator end)
    : _allocator( this->get_aspect().template get<_allocator_>()(*this) )
  {
  }

  template<typename InputIterator>
  vtree(InputIterator beg, InputIterator end, const value_compare& comp, const allocator_type&  alloc= allocator_type() )
    : _allocator(alloc)
  {
    this->get_aspect().template get<_compare_>() = comp;
  }

  vtree(const vtree& )
  {
    // TODO: Запретить копирование если работаем с файлом
    throw not_impl("vtree(const vtree& )");
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  vtree(vtree&& __x)
  {
    throw not_impl("vtree(vtree&& __x)");
    // TODO: : _M_t(std::move(__x._M_t))
  }

  vtree( std::initializer_list<value_type> il, const value_compare& comp= value_compare(), const allocator_type&  alloc= allocator_type())
    : _allocator(alloc)
  {
    throw not_impl("vtree( std::initializer_list<value_type> il, const value_compare& comp= value_compare(), const allocator_type&  alloc= allocator_type())");
    this->get_aspect().template get<_compare_>() = comp;
  }

#endif

  vtree&  operator=(const vtree& __x)
  {
    throw not_impl("vtree&  operator=(const vtree& __x)");
    return *this;
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  vtree& operator=(vtree&& __x)
  {

    throw not_impl("vtree& operator=(vtree&& __x)");
  // NB: DR 1204.
  // NB: DR 675.
  //  this->clear();
  //  this->swap(__x);
    return *this;
  }

  vtree& operator=( std::initializer_list<value_type> il)
  {
    throw not_impl("vtree& operator=( std::initializer_list<value_type> il)");
    return *this;
  }
#endif

  // accessors:

  const key_compare& key_comp() const
  {
    throw not_impl("const key_compare& key_comp() const");
  }

  const value_compare& value_comp() const
  {
    throw not_impl("const value_compare& value_comp() const");
  }

  allocator_type get_allocator() const
  {
    return _allocator;
  }

  const container_type& get_container() const
  {
    return _container;
  }

  container_type& get_container()
  {
    return _container;
  }


  iterator  begin()
  {
    return iterator( _container.begin(), 0);
  }

  iterator end()
  {
    return iterator( _container.end(), 0 );
  }

  const_iterator  begin() const
  {
    return const_iterator( _container.begin(), 0);
  }

  const_iterator end() const
  {
    return const_iterator( _container.end(), 0 );
  }

  reverse_iterator rbegin()
  {
    return reverse_iterator( this->end() );
  }

  reverse_iterator rend()
  {
    return reverse_iterator( this->begin() );
  }

  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator( this->end() );
  }

  const_reverse_iterator rend() const
  {
    return const_reverse_iterator( this->begin() );
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  const_iterator  cbegin() const
  {
    return const_iterator( _container.cbegin(), 0);
  }

  const_iterator cend() const
  {
    return const_iterator( _container.cend(), 0 );
  }

  const_reverse_iterator crbegin() const
  {
    return const_reverse_iterator( this->begin() );
  }

  reverse_iterator crend() const
  {
    return const_reverse_iterator( this->end() );
  }

#endif

  bool empty() const
  {
    return _container.empty();
  }

  size_type size() const
  {
    return this->get_aspect().template get<_size_>();
  }

  size_type max_size() const
  {
    return std::numeric_limits<size_type>::max();
  }

  void swap( vtree& s )
  {
    throw not_impl("void swap( vtree& s )");
  }


  size_t capacity() const
  {
    throw not_impl("size_t capacity() const");
  }

  iterator insert(const value_type& value)
  {
    return this->get_aspect().template get<_insert_value_>()(*this, value);
  }


#ifdef __GXX_EXPERIMENTAL_CXX0X__

//#warning

  iterator insert(value_type&& value)
  {
    return this->get_aspect().template get<_insert_value_>()(*this, value);
  }


#endif

  iterator  insert(const_iterator, const value_type& value)
  {
    throw not_impl("iterator  insert(const_iterator, const value_type& value)");
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  iterator insert(const_iterator, value_type&& value)
  {
    throw not_impl("iterator insert(const_iterator, value_type&& value)");
  }
#endif

  template<typename InputIterator>
  void insert(InputIterator beg, InputIterator end)
  {
    for (;beg!=end;++beg)
      this->insert(*beg);
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  void insert( std::initializer_list<value_type> lst)
  {
    throw not_impl("void insert( std::initializer_list<value_type> lst)");
  }

#endif

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  iterator erase(const_iterator itr)
  {
    return this->get_aspect().template get<_erase_iterator_>()(*this, itr);
  }

#else

  void erase(iterator itr)
  {
    this->get_aspect().template get<_erase_iterator_>()(*this, itr);
  }

#endif

  size_type erase(const key_type& key)
  {
    return this->get_aspect().template get<_erase_value_>()(*this, key);
    //throw not_impl("size_type erase(const key_type& /*key*/)");
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  iterator erase(const_iterator first, const_iterator last)
  {
    return this->get_aspect().template get<_erase_range_>()(*this, first, last);
  }

#else

  void erase(iterator first, iterator last)
  {
    this->get_aspect().template get<_erase_range_>()(*this, first, last);
  }

#endif

  void clear()
  {
    return this->get_aspect().template get<_clear_>()(*this);
  }

  size_type count(const key_type& key) const
  {
    throw not_impl("size_type count(const key_type& key) const");
  }

  iterator find(const key_type& /*key*/)
  {
    throw not_impl("iterator find(const key_type& /*key*/)");
  }

  const_iterator find(const key_type& key ) const
  {
    throw not_impl("const_iterator find(const key_type& key ) const");
  }

  iterator lower_bound(const key_type& key)
  {
    return this->get_aspect().template get<_lower_bound_>()(*this, key);
  }

  const_iterator lower_bound(const key_type& key) const
  {
    return this->get_aspect().template get<_lower_bound_>()(*this, key);
  }

  iterator upper_bound(const key_type& key)
  {
    return this->get_aspect().template get<_upper_bound_>()(*this, key);
  }

  const_iterator upper_bound(const key_type& key) const
  {
    return this->get_aspect().template get<_upper_bound_>()(*this, key);
  }

  std::pair<iterator, iterator> equal_range(const key_type& x)
  {
    throw not_impl("std::pair<iterator, iterator> equal_range(const key_type& x)");
  }

  std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const
  {
    throw not_impl("std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const");
  }


  template<typename AA>
  friend bool operator==(const vtree<AA>&, const vtree<AA>&);

  template<typename AA>
  friend bool operator<(const vtree<AA>&, const vtree<AA>&);

};

template<typename A>
inline bool operator==(const vtree<A>& __x, const vtree<A>& __y)
{
  throw not_impl("inline bool operator==(const vtree<A>& __x, const vtree<A>& __y)");
}

template<typename A>
inline bool operator< (const vtree<A>& __x, const vtree<A>& __y)
{
  throw not_impl("inline bool operator< (const vtree<A>& __x, const vtree<A>& __y)");
}

template<typename A>
inline bool
operator!=(const vtree<A>& __x, const vtree<A>& __y)
{
  return !(__x == __y);
}

template<typename A>
inline bool operator>(const vtree<A>& __x, const vtree<A>& __y)
{
  return __y < __x;
}

template<typename A>
inline bool operator<=(const vtree<A>& __x, const vtree<A>& __y)
{
  return !(__y < __x);
}

template<typename A>
inline bool operator>=(const vtree<A>& __x, const vtree<A>& __y)
{
  return !(__x < __y);
}

template<typename A>
inline void swap(vtree<A>& __x, vtree<A>& __y)
{
  __x.swap(__y);
}

}}

#endif
