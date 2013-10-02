//
// Author: Vladimir Migashko <migashko@wamba.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#pragma once

namespace vset{
  
template<typename A, typename C>
struct compare
{
  template<typename D>
  bool operator()(const D& l, const D& r) const
  {
    return C()( A()(l), A()(r) );
  }
};

}