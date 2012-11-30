//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_ALLOCATOR_OFFSET_POINTER_HPP
#define VSET_VSET_ALLOCATOR_OFFSET_POINTER_HPP



namespace vset { namespace memory{

template<typename T, typename M>
struct offset_pointer
{
  typedef offset_pointer<T, M> self;
  typedef M offset_provider;
  offset_provider _provider;
  size_t offset;

  typedef typename std::iterator_traits<T*>::iterator_category iterator_category;
  typedef typename std::iterator_traits<T*>::value_type        value_type;
  typedef typename std::iterator_traits<T*>::difference_type   difference_type;
  typedef typename std::iterator_traits<T*>::pointer           pointer;
  typedef typename std::iterator_traits<T*>::reference         reference;

  offset_pointer()
    : _provider()
    , offset(static_cast<size_t>(-1))
  { }

  offset_pointer(offset_provider provider, size_t offset = static_cast<size_t>(-1) )
    : _provider(provider)
    , offset(offset)
  {}

  offset_pointer(offset_provider provider, T* ptr )
    : _provider(provider)
    , offset(_provider.offset(ptr))
  {
  }
  
  operator T* () { return _get_pointer();}
  operator const T* () const { return _get_pointer();}

  T& operator*() { return *(_get_pointer());}
  const T& operator*() const { return *(_get_pointer());}
  T* operator->() { return _get_pointer(); }

  const T* operator->() const { return _get_pointer(); }

  T* _get_pointer()
  {
    if ( offset == static_cast<size_t>(-1) )
      return 0;
    return _provider.get(offset);
  }
  
  const T* _get_pointer() const
  {
    if ( offset == static_cast<size_t>(-1) )
      return 0;
    return _provider.get(offset);
  }

  void operator = ( size_t offset ) { this->offset = offset; }
  void operator = ( T* t ) { this->offset = _provider.offset(t); }
  operator size_t () const { return offset;}

  operator bool () const { return offset != static_cast<size_t>(-1);}

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
