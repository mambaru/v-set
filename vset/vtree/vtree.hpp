#ifndef VSET_VTREE_VTREE_HPP
#define VSET_VTREE_VTREE_HPP

#include <cstddef>
#include <stdexcept>
#include <map>
#include <limits>
#include <algorithm>
#include <type_traits>

#include <vset/vtree/vtree_iterator.hpp>
#include <vset/vtree/aspect/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>

#include <fas/aop.hpp>
#include <fas/static_check/static_error.hpp>

namespace vset{ namespace vtree{

struct not_impl: std::domain_error
{
  not_impl(): std::domain_error("not_impl") {}
  not_impl(const char* what): std::domain_error(what) {}
};


template<typename K, typename C>
struct compare_pair
{
  C _comp;

  compare_pair()
    : _comp()
  {}
  
  compare_pair(C comp)
    : _comp(comp)
  {
  }
  
  bool operator()(const K& first, const K& second) const
  {
    return _comp(first.first, second.first)
           || ( !_comp(second.first, first.first)
                && _comp(first.second, second.second) );
  }
};

template< typename A >
class vtree:
  public fas::aspect_class<A>
{
  typedef vtree<A> self;
  typedef fas::aspect_class<A> super;
  typedef typename super::aspect::template advice_cast<_allocator_>::type         allocator_builder;
  
public:
  typedef typename super::aspect::template advice_cast<_key_compare_>::type       key_compare;
  typedef typename super::aspect::template advice_cast<_value_compare_>::type     value_compare;
  typedef typename super::aspect::template advice_cast<_value_type_>::type        value_type;
  typedef typename super::aspect::template advice_cast<_key_type_>::type          key_type;

  typedef typename allocator_builder::template apply<self>::type                  allocator_type;
  typedef typename allocator_type::size_type           size_type;
  typedef typename allocator_type::difference_type     difference_type;

  typedef std::pair<value_type, value_type>  container_key;
  typedef compare_pair<container_key, key_compare> container_comparator;
  typedef typename super::aspect
                        ::template advice_cast<_container_>::type
                        ::template apply< container_key, typename allocator_type::pointer, container_comparator >
                        ::type container_type;

  typedef vtree_iterator<typename container_type::iterator, value_type> iterator;
  typedef vtree_iterator<typename container_type::const_iterator, const value_type> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef typename std::iterator_traits<iterator>::pointer pointer;
  typedef typename std::iterator_traits<iterator>::reference reference;
  typedef typename std::iterator_traits<const_iterator>::pointer const_pointer;
  typedef typename std::iterator_traits<const_iterator>::reference const_reference;
  
  allocator_type  _allocator;
  container_type  _container;

public:

  vtree()
    : _allocator( this->get_aspect().template get<_allocator_>()(*this) )
  {}

  explicit vtree(const key_compare& comp,const allocator_type& alloc = allocator_type() )
    : _allocator(alloc)
    , _container( container_comparator(comp) ) 
  {
    this->get_aspect().template get<_compare_>() = comp;
    _allocator = this->get_aspect().template get<_allocator_>()(*this);
  }

  template<typename InputIterator>
  vtree(InputIterator beg, InputIterator end)
    : _allocator( this->get_aspect().template get<_allocator_>()(*this) )
  {
  }

  template<typename InputIterator>
  vtree(InputIterator beg, InputIterator end, const value_compare& comp, const allocator_type&  alloc= allocator_type() )
    : _allocator(alloc)
    , _container( container_comparator(comp) ) 
  {
    this->get_aspect().template get<_compare_>() = comp;
    _allocator = this->get_aspect().template get<_allocator_>()(*this);
  }

  vtree(const vtree& __x)
    : _allocator( this->get_aspect().template get<_allocator_>()(*this) ),
      _container()
  {
    //если есть _open_file_ копирование недоступно
    struct copy_ctor_disabled_for_mapped_files;
    typename fas::static_error< copy_ctor_disabled_for_mapped_files, super::aspect::template has_advice< ::vset::buffer::persistent::_open_file_ >::value == 0 >::type error;
    this->insert(__x.begin(), __x.end());
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  vtree(vtree&& __x)
  noexcept(std::is_nothrow_copy_constructible<allocator_type>::value)
  {
    vtree tmp;
    this->swap(__x);
  }

  vtree( std::initializer_list<value_type> il, const value_compare& comp = value_compare(), const allocator_type&  alloc = allocator_type())
    : _allocator(alloc),
      _container()
  {
    this->get_aspect().template get<_compare_>() = comp;
    _allocator = this->get_aspect().template get<_allocator_>()(*this);
    for(auto item : il)
    {
      this->insert(item);
    }
  }

#endif

  vtree& operator=(const vtree& __x)
  {
    vtree tmp(__x);
    this->swap(tmp);
    return *this;
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  vtree& operator=(vtree&& __x)
  {
    this->clear();
    this->swap(__x);
    return *this;
  }

  vtree& operator=( std::initializer_list<value_type> il)
  {
    this->clear();
    this->insert(il.begin(), il.end());
    return *this;
  }
#endif

  const key_compare& key_comp() const
  {
    return this->get_aspect().template get<_key_compare_>();
  }

  const value_compare& value_comp() const
  {
    return this->get_aspect().template get<_value_compare_>();
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
    this->_container.swap(s._container);
    size_type s_size = s.get_aspect().template get<_size_>();
    s.get_aspect().template get<_size_>() = this->get_aspect().template get<_size_>();
    this->get_aspect().template get<_size_>() = s_size;
    std::swap(this->_allocator, s._allocator);
  }

  size_t capacity() const
  {
    return this->_container.size() * super::aspect::template advice_cast< ::vset::vtree::_array_type_ >::type::dimension;
  }

  iterator insert(const value_type& value)
  {
    return this->get_aspect().template get<_insert_value_>()(*this, value);
  }


#ifdef __GXX_EXPERIMENTAL_CXX0X__

  iterator insert(value_type&& value)
  {
    return this->get_aspect().template get<_insert_value_>()(*this, value );
  }

#endif

  iterator insert(const_iterator, const value_type& value)
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
    for (; beg != end; ++beg)
    {
      this->insert(*beg);
    }
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  void insert( std::initializer_list<value_type> lst)
  {
    this->insert(lst.begin(), lst.end());
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
    return this->upper_bound(key) - this->lower_bound(key);
  }

// TODO: find for c++03
  iterator find(const key_type& key)
  {
    return this->get_aspect().template get<_find_>()(*this, key);
  }

  const_iterator find(const key_type& key ) const
  {
    return this->get_aspect().template get<_find_>()(*this, key);
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
    return std::make_pair(this->lower_bound(x), this->upper_bound(x) );
  }

  std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const
  {
    return std::make_pair(this->lower_bound(x), this->upper_bound(x) );
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
