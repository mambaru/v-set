#ifndef VSET_VTREE_VTREE_ITERATOR_HPP
#define VSET_VTREE_VTREE_ITERATOR_HPP

namespace vset{ namespace vtree{

template<typename T>
struct mapped_type_helper
{
  typedef typename T::value_type value_type;
};

template<typename T>
struct mapped_type_helper<T*>
{
  typedef T value_type;
};

template<typename SourceIterator, typename ValueType>
class vtree_iterator
{
public:
  typedef vtree_iterator<SourceIterator, ValueType> self;
  typedef SourceIterator source_iterator;
  typedef typename source_iterator::value_type source_value_type;
  typedef typename source_value_type::first_type source_key_type;
  typedef typename source_value_type::second_type source_mapped_type;
  
  typedef typename mapped_type_helper<source_mapped_type>::value_type array_type;
  
  typedef typename array_type::value_type array_value_type;
  typedef typename array_type::size_type  size_type;

  typedef typename std::iterator_traits<array_value_type*>::iterator_category iterator_category;
  typedef typename std::iterator_traits<ValueType*>::value_type value_type;
  typedef typename std::iterator_traits<ValueType*>::difference_type difference_type;
  typedef typename std::iterator_traits<ValueType*>::pointer pointer;
  typedef typename std::iterator_traits<ValueType*>::reference reference;
  
  vtree_iterator()
    : _itr()
    , _pos(0)
  {

  }

  vtree_iterator(source_iterator itr, difference_type pos)
    : _itr(itr)
    , _pos(pos)
  {
  }

  vtree_iterator(const self& slf)
    : _itr(slf._itr )
    , _pos(slf._pos )
  {
  }
  
  template<typename TI, typename VT>
  vtree_iterator(const vtree_iterator<TI, VT>& slf)
    : _itr(slf.get_source_iterator() )
    , _pos(slf.get_position() )
  {
  }

  reference operator*() const
  {
    return _itr->second->at( static_cast<size_type>(_pos) );
  }
  
  pointer operator->() const
  {
    return &*(_itr->second->begin() + _pos);
  }

  self& operator++()
  {
    ++_pos;
    if ( _itr->second->size() == static_cast<size_t>(_pos))
    {
      ++_itr;
      _pos = 0;
    }
    return *this;
  }

  self operator++(int)
  {
    self ans = *this;
    this->operator++();
    return ans;
  }

  self& operator--()
  {
    if ( _pos == 0 )
    {
      --_itr;
      _pos = static_cast<difference_type>( _itr->second->size() ) - 1;
    }
    else
    {
      --_pos;
    }
    return *this;
  }

  self operator--(int)
  {
    self ans = *this;
    this->operator--();
    return ans;
  }

  self& operator += (difference_type n )
  {
    if ( n >= static_cast<difference_type>( _itr->second->size() ) - _pos )
    {
      n -= static_cast<difference_type>(_itr->second->size()) - _pos;
      ++_itr;
      _pos = 0;

      while ( n >= static_cast<difference_type>( _itr->second->size() ) )
      {
        n -= static_cast<difference_type>( _itr->second->size() );
        ++_itr;
        _pos = 0;
      }
    }
    _pos += n;
    return *this;
  }

  self& operator -= (difference_type n )
  {
    if ( n > _pos )
    {
      n -= _pos;
      --_itr;
      _pos = _itr->second->size() - 1;

      while ( n >= static_cast<difference_type>( _itr->second->size() ) )
      {
        n -= _itr->second->size();
        --_itr;
        _pos = _itr->second->size() - 1;
      }
    }
    _pos -= n;
    return *this;
  }

  bool operator == (const self& r ) const
  {
    return _pos == r._pos && _itr == r._itr;
  }

  bool operator != (const self& r ) const
  {
    return !this->operator == (r);
  }

  bool operator < (const self& r ) const
  {
    if ( _itr < r._itr ) return true;
    return _pos < r._pos;
  }

  bool operator > (const self& r ) const
  {
    if ( _itr > r._itr ) return true;
    return _pos > r._pos;
  }

  bool operator <= (const self& r ) const
  {
    return !this->operator >( r );
  }

  bool operator >= (const self& r ) const
  {
    return !this->operator < ( r );
  }

  reference operator[] ( difference_type n ) const
  {
    self ans = *this;
    ans += n;
    return *ans;
  }

  template<typename TI, typename VT>
  friend typename vtree_iterator<TI, VT>::difference_type operator - ( vtree_iterator<TI, VT> r1, vtree_iterator<TI, VT> r2 );

  source_iterator get_source_iterator() const
  {
    return this->_itr;
  }

  difference_type get_position() const
  {
    return this->_pos;
  }

private:
  source_iterator _itr;
  difference_type _pos;
};

template<typename TI, typename VT, typename Dist>
inline vtree_iterator<TI, VT> operator +
  (
    vtree_iterator<TI, VT> r,
    Dist n
  )
{
  return r+=n;
}

template<typename TI, typename VT, typename Dist>
inline vtree_iterator<TI, VT> operator +
  (
    Dist n,
    vtree_iterator<TI, VT> r
  )
{
  return r+= n;
}

template<typename TI, typename VT, typename Dist>
inline vtree_iterator<TI, VT> operator -
  (
    vtree_iterator<TI, VT> r,
    Dist n
  )
{
  return r-= n;
}

template<typename TI, typename VT, typename Dist>
inline vtree_iterator<TI, VT> operator -
  (
    Dist n,
    vtree_iterator<TI, VT> r
  )
{
  return r -= n;
}

template<typename TI, typename VT>
inline typename vtree_iterator<TI, VT>::difference_type operator -
  (
    vtree_iterator<TI, VT> r1,
    vtree_iterator<TI, VT> r2
  )
{
  typedef typename vtree_iterator<TI, VT>::difference_type difference_type;
  if ( r1._itr == r2._itr )
  {
    return  r1._pos - r2._pos;
  }

  typename vtree_iterator<TI, VT>::source_iterator titr = r2._itr;
  difference_type result =  static_cast<difference_type>(titr->second->size()) - r2._pos;
  
  for ( ++titr; titr != r1._itr; ++titr )
  {
    result += static_cast<difference_type>( titr->second->size() );
  }

  result += r1._pos;
  return result;
}

}}

#endif

