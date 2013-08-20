//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_BUFFER_PERSISTENT_PROVIDER_HPP
#define VSET_BUFFER_PERSISTENT_PROVIDER_HPP

#include <vset/buffer/provider.hpp>
#include <vset/buffer/persistent/tags.hpp>


namespace vset { namespace buffer{

template<typename T>
class persistent_provider
  : public provider<T>
{
  using provider<T>::_target;
public:
  typedef typename T::aspect::template advice_cast<_data_type_>::type data_type;
  typedef typename T::aspect::template advice_cast<_size_type_>::type size_type;

  persistent_provider(T* target)
    : provider<T>(target)
  {
  }

  void close()
  {
    _target->get_aspect().template gete< persistent::_before_close_ >()(*_target);
    _target->get_aspect().template get< persistent::_close_ >()(*_target);
    _target->get_aspect().template gete< persistent::_after_close_ >()(*_target);
  }

  void open(const char* path)
  {
    _target->get_aspect().template gete< persistent::_before_open_>()(*_target);
    _target->get_aspect().template get< persistent::_open_>()(*_target, path);
    _target->get_aspect().template gete< persistent::_after_open_>()(*_target);
  }

  size_type sync()
  {
    _target->get_aspect().template gete< persistent::_before_sync_ >()(*_target);
    size_type result = _target->get_aspect().template get<persistent::_sync_>()(*_target);
    _target->get_aspect().template gete< persistent::_after_sync_>()(*_target, result);
    return result;
  }
 
  size_type sync(size_type offset, size_type size )
  {
    _target->get_aspect().template gete< persistent::_before_sync_ >()(*_target, offset, size);
    size_type result = _target->get_aspect().template get<persistent::_sync_>()(*_target, offset, size);
    _target->get_aspect().template gete< persistent::_after_sync_>()(*_target, result);
    return result;
  }

  size_type sync(bool async)
  {
    _target->get_aspect().template gete< persistent::_before_sync_ >()(*_target, async);
    size_type result = _target->get_aspect().template get<persistent::_sync_>()(*_target, async);
    _target->get_aspect().template gete< persistent::_after_sync_>()(*_target, result, async);
    return result;
  }

  size_type sync(size_type offset, size_type size, bool async )
  {
    _target->get_aspect().template gete< persistent::_before_sync_ >()(*_target, offset, size, async);
    size_type result = _target->get_aspect().template get<persistent::_sync_>()(*_target, offset, size, async);
    _target->get_aspect().template gete< persistent::_after_sync_>()(*_target, result, async);
    return result;
  }
};

}}

#endif
