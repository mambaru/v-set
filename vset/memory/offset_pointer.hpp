//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_ALLOCATOR_OFFSET_POINTER_HPP
#define VSET_VSET_ALLOCATOR_OFFSET_POINTER_HPP

#include <iterator>
#include <fas/system/nullptr.hpp>
  
namespace vset { namespace memory{

/**
 * @brief Указатель для перемещения в буфере менеджера памяти vset::memory::manager
 */
template<typename T, typename M>
class offset_pointer
{
public:
  typedef offset_pointer<T, M> self;
  typedef M offset_provider;
  typedef T value_type;
  typedef typename std::iterator_traits<value_type*>::iterator_category iterator_category;
  typedef typename std::iterator_traits<value_type*>::difference_type   difference_type;
  typedef typename std::iterator_traits<value_type*>::pointer           pointer;
  typedef typename std::iterator_traits<value_type*>::reference         reference;

  offset_pointer()
    : _provider()
    , _offset(static_cast<size_t>(-1))
  {
  }

  explicit offset_pointer(offset_provider provider, size_t offset = static_cast<size_t>(-1) )
    : _provider(provider)
    , _offset(offset)
  {
  }

  
  operator value_type* ()
  {
    return this->get_address();
  }
  
  operator const value_type* () const
  {
    return this->get_address();
  }
  
  
  value_type& operator*()
  {
    return *(this->get_address());
  }
  
  const value_type& operator*() const
  {
    return *(this->get_address());
  }
  
  value_type* operator->()
  {
    return this->get_address();
  }

  const value_type* operator->() const
  {
    return this->get_address();
  }
  

  value_type& get_ref()
  {
    return *(this->get_address());
  }
  
  const value_type& get_ref() const
  {
    return *(this->get_address());
  }

  const value_type& get_cref() const
  {
    return *(this->get_address());
  }

  value_type* get_address()
  {
    if ( _offset == static_cast<size_t>(-1) )
      return fas_nullptr;
    return _provider.get(_offset);
  }
  
  const value_type* get_address() const
  {
    if ( _offset == static_cast<size_t>(-1) )
      return fas_nullptr;
    return _provider.get(_offset);
  }

  size_t set_address(value_type* t)
  {
    _offset = _provider.offset(t);
    return _offset;
  }

  size_t get_offset() const
  {
    return _offset;
  }

  void set_offset(size_t offset) 
  {
    _offset = offset;
  }

  bool is_null() const
  {
    return _offset == static_cast<size_t>(-1);
  }

  self& operator++()
  {
    _offset = _provider.next(_offset);
    return *this;
  }

  self operator++(int)
  {
    self ans = *this;
    _offset = _provider.next(_offset);
    return ans;
  }

  self& operator--()
  {
    _offset = _provider.pred(_offset);
    return *this;
  }

  self operator--(int)
  {
    self ans = *this;
    _offset = _provider.pred(_offset);
    return ans;
  }

  bool operator == (const self& r ) const
  {
    return _offset == r._offset && _provider==r._provider ;
  }

  bool operator != (const self& r ) const
  {
    return !this->operator == (r);
  }

  bool operator < (const self& r ) const
  {
    return _offset < r._offset;
  }

  bool operator > (const self& r ) const
  {
    return _offset > r._offset;
  }

  bool operator <= (const self& r ) const
  {
    return !this->operator >( r );
  }

  bool operator >= (const self& r ) const
  {
    return !this->operator < ( r );
  }

  self& operator += (difference_type n )
  {
    if ( n >= 0 )
      _offset = _provider.next( _offset, static_cast<size_t>(n) );
    else
      return this->operator -= ( -1 * n );
    return *this;
  }

  self& operator -= (difference_type n )
  {
    if ( n >= 0 )
      _offset = _provider.pred( _offset, static_cast<size_t>(n) );
    else
      return this->operator += ( -1 * n );
    return *this;
  }

  reference operator[] ( difference_type n )
  {
    return *(_provider.get( _provider.next( _offset, static_cast<size_t>(n) ) ));
    /*return *(_provider.get( _offset + sizeof(value_type)*n ));*/
  }

  const reference operator[] ( difference_type n ) const
  {
    return *(_provider.get( _provider.next( _offset, static_cast<size_t>(n) ) ));
    /*return *(_provider.get( _offset + sizeof(value_type)*n ));*/
  }

private:
  offset_provider _provider;
  size_t _offset;
};


template<typename T, typename M, typename Dist>
inline offset_pointer<T, M> operator +
  (
    offset_pointer<T, M> r,
    Dist n
  )
{
  return r+=n;
}

template<typename T, typename M, typename Dist>
inline offset_pointer<T, M> operator +
  (
    Dist n,
    offset_pointer<T, M> r
  )
{
  return r+= n;
}

template<typename T, typename M, typename Dist>
inline offset_pointer<T, M> operator -
  (
    offset_pointer<T, M> r,
    Dist n
  )
{
  return r-= n;
}

template<typename T, typename M, typename Dist>
inline offset_pointer<T, M> operator -
  (
    Dist n,
    offset_pointer<T, M> r
  )
{
  return r -= n;
}

template<typename T, typename M>
inline typename offset_pointer<T, M>::difference_type operator -
  (
    offset_pointer<T, M> r1,
    offset_pointer<T, M> r2
  )
{
  typename offset_pointer<T, M>::difference_type dist = 0;
  for ( ;r2!=r1; ++r2) ++dist;
  return dist;
}

}}

#endif
