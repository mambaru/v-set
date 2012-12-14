//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_BASIC_HEAD_HPP
#define VSET_VSET_BUFFER_PERSISTENT_BASIC_HEAD_HPP

namespace vset { namespace buffer{ namespace persistent{

/// Заголовок который будет записан в начало буфера
/// Может не использовать заголовок - достаточно включить в аспект fas::value_advice< _buffer_head_, empty_head >
template< size_t ID >
struct head
{
private:
  /// Идентификатор формата данных (определяеться разработчиком)
  size_t _format;
  /// Начало блока данных в буфере (сразу за заголовком)
  size_t _offset;
  /// Фактический размер данных
  size_t _size;
  /// Фактический размер буфера
  size_t _capacity;
public:
  size_t format() const { return _format; }
  size_t offset() const { return _offset; }
  size_t size() const { return _size; }
  size_t capacity() const { return _capacity; }

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
  size_t format() const { return 0; }
  size_t offset() const { return 0; }
  size_t size() const { return 0; }
  size_t capacity() const { return 0; }

  void set_size(size_t ) { }
  void set_capacity(size_t ) { }
};


}}}

#endif
