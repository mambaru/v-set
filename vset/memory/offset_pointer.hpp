//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_ALLOCATOR_OFFSET_POINTER_HPP
#define VSET_VSET_ALLOCATOR_OFFSET_POINTER_HPP

#include <iterator>

namespace vset { namespace memory{

//#warning TODO: переименовать в итератор, избавиться от T
//#warning TODO: это рандом access
  
template<typename T, typename M>
class offset_pointer
{
public:
  typedef offset_pointer<T, M> self;
  typedef M offset_provider;
  typedef /*typename offset_provider::value_type*/T value_type;
  typedef typename std::iterator_traits<value_type*>::iterator_category iterator_category;
  typedef typename std::iterator_traits<value_type*>::difference_type   difference_type;
  typedef typename std::iterator_traits<value_type*>::pointer           pointer;
  typedef typename std::iterator_traits<value_type*>::reference         reference;

  offset_pointer()
    : _provider()
    , offset(static_cast<size_t>(-1))
  { }

  explicit offset_pointer(offset_provider provider, size_t offset = static_cast<size_t>(-1) )
    : _provider(provider)
    , offset(offset)
  {}

  operator T* () { return this->get_pointer();}
  operator const T* () const { return this->get_pointer();}

  T& operator*() { return *(this->get_pointer());}
  const T& operator*() const { return *(this->get_pointer());}
  T* operator->() { return this->get_pointer(); }

  const T* operator->() const { return this->get_pointer(); }

  T* get_pointer()
  {
    if ( offset == static_cast<size_t>(-1) )
      return 0;
    return _provider.get(offset);
  }
  
  const T* get_pointer() const
  {
    if ( offset == static_cast<size_t>(-1) )
      return 0;
    return _provider.get(offset);
  }

  self& set_pointer(T* t)
  {
    this->offset = _provider.offset(t);
    return *this;
  }

  size_t get_offset() const
  {
    return offset;
  }


  void set_offset(size_t offset) 
  {
    this->offset = offset;
  }

  bool is_null() const
  {
    return offset == static_cast<size_t>(-1);
  }

  self& operator++()
  {
    offset = _provider.next(offset);
    return *this;
  }

  self operator++(int)
  {
    self ans = *this;
    offset = _provider.next(offset);
    return ans;
  }

  self& operator--()
  {
    offset = _provider.pred(offset);
    return *this;
  }

  self operator--(int)
  {
    self ans = *this;
    offset = _provider.pred(offset);
    return ans;
  }

  bool operator == (const self& r ) const
  {
    return offset == r.offset && _provider==r._provider ;
  }

  bool operator != (const self& r ) const
  {
    return !this->operator == (r);
  }

  bool operator < (const self& r ) const
  {
    return offset < r.offset;
  }

  bool operator > (const self& r ) const
  {
    return offset > r.offset;
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
    offset = _provider.next(offset, n);
    return *this;
  }

  self& operator -= (difference_type n )
  {
    offset = _provider.pred(offset, n);
    return *this;
  }

  reference operator[] ( difference_type n )
  {
    return *(_provider.get( offset + sizeof(T)*n ));
  }

  const reference operator[] ( difference_type n ) const
  {
    return *(_provider.get( offset + sizeof(T)*n ));
  }

private:
  offset_provider _provider;
  size_t offset;
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
