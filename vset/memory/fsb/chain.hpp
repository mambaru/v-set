//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_MEMORY_FSB_CHAIN_HPP
#define VSET_MEMORY_FSB_CHAIN_HPP

namespace vset { namespace memory{ namespace fsb{

template<typename T, template<typename> class Chunk >
struct chain
{
  typedef chain<T, Chunk> self;
  typedef Chunk<T> chunk_type;

  size_t size;
  mutable size_t first_free;

  chain()
    : size(0)
    , first_free(0)
  {}

  void acquire(size_t count)
  {
    size += count;
  }

  chunk_type* first_chunk()
  {
    return reinterpret_cast<chunk_type*>(this+1);
  }

  const chunk_type* first_chunk() const
  {
    return reinterpret_cast<const chunk_type*>(this+1);
  }

  chunk_type* last_chunk()
  {
    chunk_type* cnk = reinterpret_cast<chunk_type*>(this + 1);
    return size > 0 ? cnk + size - 1 : cnk;
  }

  const chunk_type* last_chunk() const
  {
    const chunk_type* cnk = reinterpret_cast<const chunk_type*>(this + 1);
    return size > 0 ? cnk + size - 1 : cnk;
  }

  
  size_t chunk_size() const
  {
    return sizeof(chunk_type);
  }

  const T* first_value() const
  {
    if ( const chunk_type* beg = first_occuped() )
    {
      return beg->first_value();
    }
    return 0;
  }

  T* first_value()
  {
    return const_cast<T*>( const_cast<const self*>(this)->first_value() );
  }

  const T* last_value() const
  {
    if ( const chunk_type* beg = last_occuped() )
    {
      return beg->last_value();
    }
    return 0;
  }

  T* last_value()
  {
    return const_cast<T*>( const_cast<const self*>(this)->last_value() );
  }
  
  const T* next_value(const T* value)  const
  {
    size_t offset = static_cast<size_t>(
      reinterpret_cast<const char*>( value               ) - 
      reinterpret_cast<const char*>( this->first_chunk() )
    );
    const chunk_type* chk = first_chunk() + offset/sizeof(chunk_type);

    if ( const T* result =  chk->next_value(value) )
    {
      return result;
    }

    for ( ++chk; chk->empty(); ++chk)
    {
      if ( static_cast<size_t>(chk - this->first_chunk()) == size )
      {
        return 0;
      }
    }

    return chk->first_value();
  }

  T* next_value(T* value)
  {
    return const_cast<T*>( const_cast<const self*>(this)->next_value(value) );
  }

  const T* pred_value(const T* value)  const
  {
    size_t offset = static_cast<size_t>(
      reinterpret_cast<const char*>(value) - 
      reinterpret_cast<const char*>(this->first_chunk())
    );
    const chunk_type* chk = first_chunk() + offset/sizeof(chunk_type);

    if ( const T* result =  chk->pred_value(value) )
    {
      return result;
    }

    if ( chk == this->first_chunk() )
    {
      return 0;
    }

    for ( --chk; chk->empty(); --chk)
    {
      if ( chk == this->first_chunk() )
      {
        return 0;
      }
    }
    return chk->last_value();
  }

  T* pred_value(T* value)
  {
    return const_cast<T*>( const_cast<const self*>(this)->pred_value(value) );
  }

  const chunk_type* first_occuped() const
  {
    const chunk_type* beg = first_chunk();
    const chunk_type* end = beg + size;

    for ( ;beg!=end; ++beg)
    {
      if ( !beg->empty() )
      {
        return beg;
      }
    }
    return 0;
  }

  chunk_type* first_occuped()
  {
    return const_cast<chunk_type*>( const_cast<const self*>(this)->first_occuped() );
  }

  const chunk_type* last_occuped() const
  {
    const chunk_type* beg = last_chunk();
    const chunk_type* end = first_chunk() - 1;

    for ( ;beg!=end; --beg)
    {
      if ( !beg->empty() )
      {
        return beg;
      }
    }
    return 0;
  }

  chunk_type* last_occuped()
  {
    return const_cast<chunk_type*>( const_cast<const self*>(this)->last_occuped() );
  }
  
  chunk_type* find_free()
  {
    chunk_type* beg = first_chunk() + first_free;
    chunk_type* end = first_chunk() + size;

    for ( ;beg!=end; ++beg)
    {
      if ( !beg->filled() )
      {
        first_free = static_cast<size_t>( beg - first_chunk() );
        return beg;
      }
    }

    first_free = static_cast<size_t>( beg - first_chunk() );
    return 0;
  }

  const chunk_type* find_free() const
  {
    const chunk_type* beg = first_chunk() + first_free ;
    const chunk_type* end = first_chunk() + size;

    for ( ;beg!=end; ++beg)
    {
      if ( !beg->filled() )
      {
        first_free = beg - first_chunk();
        return beg;
      }
    }
    first_free = beg - first_chunk();
    return 0;
  }

  T* mark()
  {
    if ( chunk_type* chk = find_free() )
    {
      return chk->mark();
    }
    return 0;
  }

  void free(T* value)
  {
    size_t offset = static_cast<size_t>(
      reinterpret_cast<char*>(value) -
      reinterpret_cast<char*>(this->first_chunk())
    );

    chunk_type* chk = this->first_chunk() + offset/sizeof(chunk_type);
    chk->free(value);

    offset = static_cast<size_t>(chk - this->first_chunk() );
    if ( offset < first_free )
    {
      first_free = offset;
    }
  }

  size_t count() const
  {
    size_t cnt=0;
    const chunk_type* beg = first_chunk();
    const chunk_type* end = first_chunk() + size;
    for (; beg != end; ++beg)
    {
      cnt+=beg->count();
    }
    return cnt;
  }

  size_t capacity() const
  {
    return size * chunk_type::max_count();
  }
};

}}}

#endif
