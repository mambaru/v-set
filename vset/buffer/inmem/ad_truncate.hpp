//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_INMEM_AD_TRUNCATE_HPP
#define VSET_VSET_BUFFER_INMEM_AD_TRUNCATE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/inmem/tags.hpp>

namespace vset { namespace buffer { namespace inmem{

struct ad_truncate
{
  template<typename T>
  void operator()( T& t, typename T::aspect::template advice_cast<_size_type_>::type size )
  {
    if ( size >  t.get_aspect().template get<_container_>().size() )
      t.get_aspect().template get<_container_>().reserve(size);
    else if ( size <  t.get_aspect().template get<_container_>().size() )
    {
      typedef typename T::aspect::template advice_cast<_container_>::type container;
      container(
        t.get_aspect().template get<_container_>().begin(),
        t.get_aspect().template get<_container_>().begin() + size
      ).swap( t.get_aspect().template get<_container_>() );
      
    }
    else if ( size <  t.get_aspect().template get<_container_>().capacity() )
    {
      typedef typename T::aspect::template advice_cast<_container_>::type container;
      container(
        t.get_aspect().template get<_container_>().begin(),
        t.get_aspect().template get<_container_>().end()
      ).swap( t.get_aspect().template get<_container_>() );
    }
  }
};

}}}

#endif
