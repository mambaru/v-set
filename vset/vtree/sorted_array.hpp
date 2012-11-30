#ifndef VSET_VTREE_SORTED_ARRAY_HPP
#define VSET_VTREE_SORTED_ARRAY_HPP

#include <vset/vtree/array.hpp>

namespace vset{ namespace vtree{

// TODO: сделать операции < > и пр.
template<typename T, size_t N, typename Compare = std::less<T> >
class sorted_array
  : public array<T, N>
{
  typedef array<T, N> super;

public:
  using super::erase;
  typedef Compare key_compare;
  typedef Compare value_compare;


  typedef typename super::value_type value_type;
  typedef typename super::data_type data_type;
  typedef typename super::size_type size_type;

  typedef typename super::reference reference;
  typedef typename super::const_reference const_reference;

  typedef typename super::pointer pointer;
  typedef typename super::const_pointer const_pointer;

  typedef typename super::const_iterator const_iterator;
  typedef typename super::iterator iterator;

  typedef typename super::reverse_iterator reverse_iterator;
  typedef typename super::const_reverse_iterator const_reverse_iterator;

  typedef std::ptrdiff_t difference_type;


  sorted_array( )
    : super()
  {};

  void resize ( size_type sz, T value = value_type(), const value_compare& comp = value_compare() )
  {
    super::resize(sz, value);
    this->sort(comp);
  }

  template <class InputIterator>
  void assign( InputIterator first, InputIterator last, const value_compare& comp = value_compare() )
  {
    super::assign(first, last);
    this->sort(comp);
  }

  void push_back ( const T& x, const value_compare& comp = value_compare() )
  {
    super::push_back(x);
    this->sort(comp);
  }

  iterator insert ( const T& x, const value_compare& comp = value_compare() )
  {
    iterator position = std::upper_bound(super::begin(), super::end(), x, comp );
    return super::insert(position, x);
  }

  void insert ( size_type n, const T& x, const value_compare& comp = value_compare() )
  {
    iterator position = std::upper_bound(super::begin(), super::end(), x, comp );
    return super::insert(position, n, x);
  }

  size_type erase( const T& x, const value_compare& comp = value_compare() )
  {
    size_type count = 0;
    std::pair<iterator, iterator> range = std::equal_range(super::begin(), super::end(), x, comp );
    for (;range.first!=range.second;++range.first, ++count)
      super::erase(range.first);
    return count;
  }

  template <class InputIterator>
  void insert ( InputIterator first, InputIterator last, const value_compare& comp = value_compare() )
  {
    super::insert( super::end(), first, last );
    this->sort(comp);
  }

  void sort(const value_compare& comp = value_compare())
  {
    std::sort( super::begin(), super::end(), comp );
  }

private:
  
};


}}

#endif