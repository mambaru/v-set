//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_HEAD_HPP
#define VSET_VSET_BUFFER_PERSISTENT_HEAD_HPP

#include <cstddef>

namespace vset { namespace buffer{ namespace persistent{

template< size_t ID >
struct head
{
  
private:
  size_t _format;
  size_t _offset;
  size_t _size;
  size_t _capacity;
  
public:
  
  size_t format() const { return _format; }
  size_t offset() const { return _offset; }
  size_t size() const { return _size; }
  size_t capacity() const { return _capacity; }
  void reset()
  {
    _size = 0;
    _capacity=0;
  }

  void set_size(size_t size) { _size = size; }
  void set_capacity(size_t capacity) { _capacity = capacity; }

  head()
    : _format(ID)
    , _offset( sizeof(head<ID>) )
    , _size(0)
    , _capacity(0)
  {}
};

struct empty_head
{
public:
  static size_t format()  { return 0; }
  static size_t offset()  { return 0; }
  static size_t size()  { return 0; }
  static size_t capacity()  { return 0; }

  static void set_size(size_t ) { }
  static void set_capacity(size_t ) { }
};


}}}

#endif
