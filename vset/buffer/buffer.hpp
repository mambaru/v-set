//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_HPP
#define VSET_VSET_BUFFER_HPP

#include <vset/buffer/buffer_base.hpp>
#include <fas/aop.hpp>

namespace vset { namespace buffer{

template<typename A /*= fas::aspect<>*/ >
class buffer
  : public buffer_base<A>
{
  typedef buffer_base<A> super;
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
  
  void resize(size_type size)
  {
    super::_resize(*this, size);
  }

  void truncate(size_type size)
  {
    super::_truncate(*this, size);
  }

  void reserve( size_type size)
  {
    super::_reserve(*this, size);
  }
};

}}

#endif
