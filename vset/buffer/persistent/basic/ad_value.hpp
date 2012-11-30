//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_BASIC_AD_VALUE_HPP
#define VSET_VSET_BUFFER_PERSISTENT_BASIC_AD_VALUE_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>

namespace vset { namespace buffer { namespace persistent{

template<typename TTg, typename Tg>
struct ad_value
{
  template<typename T>
  typename T::aspect::template advice_cast<TTg>::type
  operator()( T& t ) const
  {
    return t.get_aspect().template get<Tg>();
  }
};

}}}

#endif
