//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_MEMORY_PROVIDER_HPP
#define VSET_MEMORY_PROVIDER_HPP

#include <vset/memory/tags.hpp>
#include <fas/typemanip/type2type.hpp>

namespace vset { namespace memory{

template<typename Manager >
class provider
{
public:
  typedef Manager manager;

  typedef typename manager::aspect::template advice_cast<_value_type_>::type value_type;

  typedef typename manager::aspect::template advice_cast<_buffer_provider_>::type
                                  ::type buffer_metatype;

  typedef typename manager::aspect::template advice_cast<_pointer_provider_>::type
                                  ::type pointer_metatype;

  typedef typename fas::apply<buffer_metatype, Manager>::type buffer_type;
  typedef typename fas::apply<pointer_metatype, value_type, Manager >::type pointer;
  typedef typename fas::apply<pointer_metatype, const value_type, const Manager >::type const_pointer;

  typedef value_type& reference;
  typedef const value_type& const_reference;

  typedef size_t size_type;
  typedef std::ptrdiff_t difference_type;

  /*
  typedef typename manager::value_type  value_type;
  typedef typename manager::buffer_type buffer_type;
  typedef typename manager::pointer pointer;
  typedef typename manager::const_pointer const_pointer;
  typedef typename manager::reference       reference;
  typedef typename manager::const_reference const_reference;
  typedef typename manager::size_type size_type;
  typedef typename manager::difference_type difference_type;
  */

  provider(): _manager(0) {}
  
  provider(Manager* m): _manager(m) {}

  pointer begin()
  {
    return _manager->get_aspect().template get<_begin_>()(*_manager, fas::type2type<pointer>() );
    //return _manager->begin();
  }

  const_pointer begin() const
  {
    return _manager->get_aspect().template get<_begin_>()(*_manager, fas::type2type<const_pointer>() );
    //return _manager->begin();
  }

  pointer end()
  {
    return _manager->get_aspect().template get<_end_>()(*_manager, fas::type2type<pointer>() );
    //return _manager->end();
  }

  const_pointer end() const
  {
    return _manager->get_aspect().template get<_end_>()(*_manager, fas::type2type<const_pointer>() );
    // return _manager->end();
  }

  pointer allocate(size_t num, void *  hint = 0)
  {
    return _manager->get_aspect().template get<_allocate_>()(*_manager, fas::type2type<pointer>(), num, hint );
    //return _manager->allocate(num, hint);
  }

  void deallocate(pointer ptr, size_type num)
  {
    return _manager->get_aspect().template get<_deallocate_>()(*_manager, ptr, num );
    //return _manager->deallocate(ptr, num);
  }

  buffer_type buffer()
  {
    return buffer_type(_manager);
    //return _manager->buffer();
  }

  const buffer_type buffer() const
  {
    return buffer_type(_manager);
    //return _manager->buffer();
  }
  
private:
  
  manager* _manager;
};

}}



#endif
