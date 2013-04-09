#ifndef VSET_VTREE_SORTED_ARRAY_HPP
#define VSET_VTREE_SORTED_ARRAY_HPP

#include <vset/array.hpp>

namespace vset{

// TODO: сделать операции < > и пр.
template<typename T, size_t N, typename Compare = std::less<T> >
class sorted_array
  : public array<T, N>
{
  typedef array<T, N> super;

public:
  
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

  void resize ( size_type sz, T value /*= value_type()*/, const value_compare& comp /*= value_compare()*/ )
  {
    bool f = this->size() < sz;
    super::resize(sz, value);
    if ( f ) this->sort(comp);
  }

  template <class InputIterator>
  void assign( InputIterator first, InputIterator last, const value_compare& comp /*= value_compare()*/ )
  {
    super::assign(first, last);
    this->sort(comp);
  }

  void push_back ( const T& x, const value_compare& comp /*= value_compare()*/ )
  {
    super::push_back(x);
    this->sort(comp);
  }

  iterator insert ( const T& x, const value_compare& comp /*= value_compare()*/ )
  {
    iterator position = std::upper_bound(super::begin(), super::end(), x, comp );
    return super::insert(position, x);
  }

  
  void insert ( size_type n, const T& x, const value_compare& comp /*= value_compare()*/ )
  {
    iterator position = std::upper_bound(super::begin(), super::end(), x, comp );
    return super::insert(position, n, x);
  }
  

  template <class InputIterator>
  void insert ( InputIterator first, InputIterator last, const value_compare& comp /*= value_compare()*/ )
  {
    super::insert( super::end(), first, last );
    this->sort(comp);
  }

  //using super::erase;
  //using super::cbegin;
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  iterator erase ( const_iterator position, const value_compare& comp )
  {
    if ( position == this->cend() )
      throw std::out_of_range("iterator array<>::erase ( iterator position )");

    iterator dst = this->begin() + std::distance(this->cbegin(), position);
    std::copy( position + 1, this->cend(), dst);
    this->resize( this->size() - 1, value_type(), comp );
    return dst;
  }
#else
  iterator erase ( iterator position, const value_compare& comp )
  {
    if ( position == this->end() )
      throw std::out_of_range("iterator array<>::erase ( iterator position )");

    std::copy( position + 1, this->end(), position);
    this->resize( this->size() - 1, value_type(), comp );
    return position;
  }
#endif

private:
  /// BUG 
  size_type erase( const T& x, const value_compare& comp )
  {
    size_type count = 0;
    std::pair<iterator, iterator> range = std::equal_range(super::begin(), super::end(), x, comp );
    for (;range.first!=range.second;++range.first, ++count)
      super::erase(range.first);
    return count;
  }
public:

  void sort(const value_compare& comp )
  {
    std::sort( super::begin(), super::end(), comp );
  }
private:
};

}

#endif