//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_BUFFER_BASE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_BUFFER_BASE_HPP

#include <vset/buffer/buffer_base.hpp>
#include <vset/buffer/persistent/file/aspect.hpp>
#include <vset/buffer/tags.hpp>
#include <fas/aop.hpp>
namespace vset { namespace buffer{

template<typename A = fas::aspect<> >
class persistent_buffer_base
  : public buffer_base< typename fas::aspect_merge<A, persistent::file::aspect>::type >
{
  typedef buffer_base< typename fas::aspect_merge<A, persistent::file::aspect>::type > super;
public:

  typedef typename super::size_type size_type;
  typedef typename super::data_type data_type;

  template<typename T>
  void _close(T& t)
  {
    t.get_aspect().template gete< persistent::_before_close_ >()(t);
    t.get_aspect().template get<persistent::_close_>()(t);
    t.get_aspect().template gete< persistent::_after_close_ >()(t);
  }

  template<typename T>
  void _open(T& t, const char* path)
  {
    t.get_aspect().template gete< persistent::_before_open_ >()(t);
    t.get_aspect().template get<persistent::_open_>()(t, path);
    t.get_aspect().template gete< persistent::_after_open_ >()(t);
  }

  template<typename T>
  size_type _sync(T& t)
  {
    t.get_aspect().template gete< persistent::_before_sync_ >()(t);
    size_type result = t.get_aspect().template get<persistent::_sync_>()(t);
    t.get_aspect().template gete< persistent::_after_sync_>()(t, result);
    return result;
  }

  template<typename T>
  size_type _sync(T& t, size_type offset, size_type size )
  {
    t.get_aspect().template gete< persistent::_before_sync_ >()(t, offset, size);
    size_type result = t.get_aspect().template get<persistent::_sync_>()(t, offset, size);
    t.get_aspect().template gete< persistent::_after_sync_>()(t, result);
    return result;
  }
};

}}

#endif
