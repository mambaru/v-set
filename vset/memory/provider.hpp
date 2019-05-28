//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_MEMORY_PROVIDER_HPP
#define VSET_MEMORY_PROVIDER_HPP

#include <vset/memory/tags.hpp>
#include <fas/typemanip/type2type.hpp>
#include <vset/memory/aspect_maker.hpp>
#include <fas/system/nullptr.hpp>

namespace vset { namespace memory{

template<typename Manager >
class provider
{
public:
  typedef Manager manager;

  typedef typename aspect_maker<manager>::value_type      value_type;
  typedef typename aspect_maker<manager>::buffer_type     buffer_type;
  typedef typename aspect_maker<manager>::pointer         pointer;
  typedef typename aspect_maker<manager>::const_pointer   const_pointer;
  typedef typename aspect_maker<manager>::difference_type difference_type;
  
  typedef value_type& reference;
  typedef const value_type& const_reference;

  typedef size_t size_type;

  provider(): _manager(fas_nullptr) {}
  
  explicit provider(Manager* m): _manager(m) {}

  pointer begin()
  {
    return _manager->get_aspect().template get<_begin_>()(*_manager, fas::type2type<pointer>() );
  }

  const_pointer begin() const
  {
    return _manager->get_aspect().template get<_begin_>()(*_manager, fas::type2type<const_pointer>() );
  }

  pointer end()
  {
    return _manager->get_aspect().template get<_end_>()(*_manager, fas::type2type<pointer>() );
  }

  const_pointer end() const
  {
    return _manager->get_aspect().template get<_end_>()(*_manager, fas::type2type<const_pointer>() );
  }

  pointer allocate(size_t num, void *  hint = fas_nullptr)
  {
    return _manager->get_aspect().template get<_allocate_>()(*_manager, fas::type2type<pointer>(), num, hint );
  }

  void deallocate(pointer ptr, size_type num)
  {
    return _manager->get_aspect().template get<_deallocate_>()(*_manager, ptr, num );
  }

  buffer_type buffer()
  {
    return buffer_type(_manager);
  }

  const buffer_type buffer() const
  {
    return buffer_type(_manager);
  }

  size_type count() const
  {
    return _manager->get_aspect().template get<_count_>()(*_manager);
  }

  size_type capacity() const
  {
    return _manager->get_aspect().template get<_capacity_>()(*_manager);
  }

private:
  
  manager* _manager;
};

}}



#endif
