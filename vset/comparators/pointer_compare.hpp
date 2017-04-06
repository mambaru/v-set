//
// Author: Vladimir Migashko <migashko@wamba.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef COMPARATORS_POINTER_COMPARE_HPP
#define COMPARATORS_POINTER_COMPARE_HPP

namespace vset{
  
template<typename C>
class pointer_compare: C
{
public:
  template<typename D>
  bool operator()(const D& l, const D& r) const
  {
    return C::operator()( *l, *r);
  }
};

}

#endif
