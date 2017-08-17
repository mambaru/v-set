//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_BUFFER_HPP
#define VSET_VSET_BUFFER_PERSISTENT_BUFFER_HPP

#include <vset/buffer/persistent/filesync/aspect.hpp>
#include <vset/buffer/persistent_buffer_base.hpp>
#include <fas/aop.hpp>

namespace vset { namespace buffer{

template< typename A >
class persistent_buffer
  : public persistent_buffer_base<A>
{
  typedef persistent_buffer_base<A> super;
public:

  typedef typename super::data_type data_type;
  typedef typename super::size_type size_type;

  size_type size() const
  {
    return super::_size(*this);
  }

  size_type capacity() const
  {
    return super::_capacity(*this);
  }

  data_type data()
  {
    return super::_data(*this);
  }

  const data_type data() const
  {
    return super::_data(*this);
  }

  void clear()
  {
    super::_clear(*this);
  }
  
  void resize(size_type s)
  {
    super::_resize(*this, s);
  }

  void truncate(size_type s)
  {
    super::_truncate(*this, s);
  }

  void reserve( size_type s)
  {
    super::_reserve(*this, s);
  }

  void close()
  {
    super::_close(*this);
  }

  void open(const char* path)
  {
    super::_open(*this, path);
  }

  size_type sync()
  {
    return super::_sync(*this);
  }

  size_type sync(size_type offset, size_type s )
  {
    return super::_sync(*this, offset, s);
  }

  size_type sync(bool syn)
  {
    return super::_sync(*this, syn);
  }

  size_type sync(size_type offset, size_type s, bool syn )
  {
    return super::_sync(*this, offset, s, syn);
  }
};

}}

#endif
