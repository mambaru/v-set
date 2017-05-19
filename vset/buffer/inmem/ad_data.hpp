//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_INMEM_AD_DATA_HPP
#define VSET_VSET_BUFFER_INMEM_AD_DATA_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/inmem/tags.hpp>

namespace vset { namespace buffer { namespace inmem{

struct ad_data
{
  template<typename T>
  const typename T::aspect::template advice_cast<_const_data_type_>::type 
  operator()( T& t ) const
  {
     return &(t.get_aspect().template get<_container_>()[0]);
  }

  template<typename T>
  typename T::aspect::template advice_cast<_data_type_>::type
  operator()( T& t )
  {
    return &(t.get_aspect().template get<_container_>()[0]);
  }
};

}}}

#endif
