//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_MEMORY_FSB_CHUNK_HPP
#define VSET_MEMORY_FSB_CHUNK_HPP

namespace vset { namespace memory{ namespace fsb{

template<typename T>
struct chunk
{
  typedef T value_type;

  size_t bits;
  T data[sizeof(size_t)*8];

  chunk(): bits(0), data() {}

  bool filled() const
  {
    return bits == static_cast<size_t>(-1);
  }

  bool empty() const
  {
    return bits == 0;
  }

  size_t max_count() const
  {
    return sizeof(size_t)*8;
  }

  size_t size() const
  {
    return sizeof(this);
  }

  void clear()
  {
    bits = 0;
  }

  T* first_value() 
  {
    size_t index = next_occuped(0);
    if ( index == static_cast<size_t>(-1) )
      return 0;
    return data + index;
  }

  const T* first_value() const
  {
    size_t index = next_occuped(0);
    if ( index == static_cast<size_t>(-1) )
      return 0;
    return data + index;
  }

  T* next_value(T* current)
  {
    size_t index = next_occuped(current - data + 1);
    if ( index == static_cast<size_t>(-1) )
      return 0;
    return data + index;
  }

  const T* next_value(const T* current) const
  {
    size_t index = next_occuped(current - data + 1);
    if ( index == static_cast<size_t>(-1) )
      return 0;
    return data + index;
  }
  
  size_t next_occuped(size_t pos = 0) const
  {
    for ( size_t i = pos; i < sizeof(size_t)*8; ++i )
      if ( bits & ( static_cast<size_t>(1) << i) )
        return i;
    return static_cast<size_t>(-1);
  }

  size_t first_free() const
  {
    for ( size_t i = 0; i < sizeof(size_t)*8; ++i )
      if ( ! ( bits & ( static_cast<size_t>(1) << i) ) )
        return i;
    return static_cast<size_t>(-1);
  }

  T* mark()
  {
    size_t index = first_free();
    if ( index == static_cast<size_t>(-1) )
      return 0;
    return mark(index);
  }

  T* mark(size_t index)
  {
    bits |= ( static_cast<size_t>(1) <<  index );
    return data + index;
  }

  void free(T* addr)
  {
    size_t index = addr - data;
    if ( index < 64 )
      bits &= ~( static_cast<size_t>(1)<<index);
    else
      throw std::invalid_argument("chunk<T>::free");
  }
};

}}}

#endif
