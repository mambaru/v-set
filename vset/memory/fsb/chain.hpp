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

  size_t chunk_size() const
  {
    return sizeof(chunk_type);
  }

  T* first_value()
  {
    if ( chunk_type* beg = first_occuped() )
      return beg->first_value();
    return 0;
  }

  const T* first_value() const
  {
    if ( const chunk_type* beg = first_occuped() )
      return beg->first_value();
    return 0;
  }
  
  T* next_value(T* value)
  {
    size_t offset = reinterpret_cast<char*>(value) - reinterpret_cast<char*>(this->first_chunk());
    chunk_type* chk = first_chunk() + offset/sizeof(chunk_type);
    T* result =  chk->next_value(value);
    if ( result == 0)
    {
      ++chk;
      if ( static_cast<size_t>(chk - this->first_chunk()) == size )
        return 0;
      result = chk->first_value();
    }
    return result;

  }

  const T* next_value(const T* value)  const
  {
    size_t offset = reinterpret_cast<const char*>(value) - reinterpret_cast<const char*>(this->first_chunk());
    const chunk_type* chk = first_chunk() + offset/sizeof(chunk_type);
    const T* result =  chk->next_value(value);
    if ( result == 0)
    {
      ++chk;
      if ( static_cast<size_t>(chk - this->first_chunk()) == size )
        return 0;
      result = chk->first_value();
    }
    return result;
  }

  chunk_type* first_occuped()
  {
    chunk_type* beg = first_chunk();
    chunk_type* end = beg + size;

    for ( ;beg!=end; ++beg)
    {
      if ( !beg->empty() )
        return beg;
    }
    return 0;
  }

  const chunk_type* first_occuped() const
  {
    const chunk_type* beg = first_chunk();
    const chunk_type* end = beg + size;

    for ( ;beg!=end; ++beg)
    {
      if ( !beg->empty() )
        return beg;
    }
    return 0;
  }

  chunk_type* find_free()
  {
    chunk_type* beg = first_chunk();
    chunk_type* end = beg + size;

    if ( beg!=end && !( beg + first_free)->filled() )
      return beg + first_free;

    for ( ;beg!=end; ++beg)
    {
      if ( !beg->filled() )
      {
        first_free = beg - first_chunk();
        return beg;
      }
    }
    return 0;
  }

  const chunk_type* find_free() const
  {
    const chunk_type* beg = first_chunk();
    const chunk_type* end = beg + size;

    if ( beg!=end && !( beg + first_free)->filled() )
      return beg + first_free;

    for ( ;beg!=end; ++beg)
    {
      if ( !beg->filled() )
      {
        first_free = beg - first_chunk();
        return beg;
      }
    }
    return 0;
  }

  T* mark()
  {
    if ( chunk_type* chk = find_free() )
      return chk->mark();
    return 0;
  }

  void free(T* value)
  {
    size_t offset = reinterpret_cast<char*>(value) - reinterpret_cast<char*>(this->first_chunk());
    chunk_type* chk = first_chunk() + offset/sizeof(chunk_type);
    chk->free(value);

    offset = chk - first_chunk();
    if ( offset < first_free )
      first_free = offset;
  }
};

}}}

#endif
