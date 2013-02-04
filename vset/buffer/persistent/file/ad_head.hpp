//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_BASIC_AD_HEAD_HPP
#define VSET_VSET_BUFFER_PERSISTENT_BASIC_AD_HEAD_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>

namespace vset { namespace buffer { namespace persistent{ namespace file{

struct ad_head
{
  template<typename T>
  const typename T::aspect::template advice_cast<_head_type_>::type*
  operator()( T& t ) const
  {
    typedef typename T::aspect::template advice_cast<_head_type_>::type head_type;
    return reinterpret_cast<const head_type*>( t.get_aspect().template get<_buffer_>() );
  }

  template<typename T>
  typename T::aspect::template advice_cast<_head_type_>::type*
  operator()( T& t )
  {
    typedef typename T::aspect::template advice_cast<_head_type_>::type head_type;
    return reinterpret_cast<head_type*>(t.get_aspect().template get<_buffer_>());
  }

};

}}}}

#endif
