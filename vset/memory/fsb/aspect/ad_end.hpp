//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_AD_END_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_AD_END_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

#include <fas/typemanip/type2type.hpp>

namespace vset { namespace memory{ namespace fsb{

struct ad_end
{
  template<typename T, typename Pointer>
  Pointer operator()(T& t, fas::type2type<Pointer> ) const
  {
    return Pointer( &t );
  }
};


}}}

#endif
