//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_MEMORY_FSB_CHUNK_HPP
#define VSET_MEMORY_FSB_CHUNK_HPP

#include <stdexcept>

namespace vset { namespace memory{ namespace fsb{

template<typename T>
struct chunk
{
  typedef chunk<T> self;
  typedef T value_type;

  size_t bits;
  T data[sizeof(size_t)*8];

  chunk()
    : bits(0)
    , data()
  {}

  bool filled() const
  {
    return bits == static_cast<size_t>(-1);
  }

  bool empty() const
  {
    return bits == 0;
  }

  static size_t max_count()
  {
    return sizeof(size_t)*8;
  }

  size_t count() const
  {
    if ( this->empty() )
    {
      return 0;
    }
    else if ( this->filled() )
    {
      return this->max_count();
    }

    size_t cnt = 0;
    for ( size_t i = 0; i < sizeof(size_t)*8; ++i )
    {
      cnt += ( 0 != (bits & ( static_cast<size_t>(1) << i)) );
    }
    return cnt;
  }

  size_t size() const
  {
    return sizeof(this);
  }

  void clear()
  {
    bits = 0;
  }

  const T* first_value() const
  {
    size_t index = next_occuped(0);
    if ( index == static_cast<size_t>(-1) )
    {
      return 0;
    }
    return data + index;
  }

  T* first_value()
  {
    return const_cast<T*>( const_cast<const self*>(this)->first_value()  );
  }

  const T* last_value() const
  {
    size_t index = pred_occuped( max_count() - 1 );
    if ( index == static_cast<size_t>(-1) )
    {
      return 0;
    }
    return data + index;
  }

  T* last_value()
  {
    return const_cast<T*>( const_cast<const self*>(this)->last_value()  );
  }

  const T* next_value(const T* current) const
  {
    size_t index = this->next_occuped( static_cast<size_t>(current - data + 1) );
    if ( index == static_cast<size_t>(-1) )
    {
      return 0;
    }
    return data + index;
  }

  T* next_value(T* current)
  {
    return const_cast<T*>( const_cast<const self*>(this)->next_value(current) );
  }

  const T* pred_value(const T* current) const
  {
    size_t index = this->pred_occuped( static_cast<size_t>(current - data - 1) );
    if ( index == static_cast<size_t>(-1) )
    {
      return 0;
    }
    return data + index;
  }

  T* pred_value(T* current)
  {
    return const_cast<T*>( const_cast<const self*>(this)->pred_value(current) );
  }
  
  size_t next_occuped(size_t pos = 0) const
  {
    for ( size_t i = pos; i < sizeof(size_t)*8; ++i )
    {
      if ( bits & ( static_cast<size_t>(1) << i) )
      {
        return i;
      }
    }
    return static_cast<size_t>(-1);
  }

  size_t pred_occuped(size_t pos = max_count() - 1) const
  {
    for ( size_t i = pos ; i < max_count(); --i )
    {
      if ( bits & ( static_cast<size_t>(1) << i) )
      {
        return i;
      }
    }
    return static_cast<size_t>(-1);
  }

  size_t first_free() const
  {
    for ( size_t i = 0; i < sizeof(size_t)*8; ++i )
    {
      if ( ! ( bits & ( static_cast<size_t>(1) << i) ) )
      {
        return i;
      }
    }
    return static_cast<size_t>(-1);
  }

  T* mark()
  {
    size_t index = first_free();
    if ( index == static_cast<size_t>(-1) )
    {
      return 0;
    }
    return mark(index);
  }

  T* mark(size_t index)
  {
    bits |= ( static_cast<size_t>(1) <<  index );
    return data + index;
  }

  void free(T* addr)
  {
    size_t index = static_cast<size_t>(addr - data);
    if ( index < 64 )
    {
      bits &= ~( static_cast<size_t>(1)<<index);
    }
    else
    {
      throw std::invalid_argument("chunk<T>::free");
    }
  }
};

}}}

#endif
