//
// Author: Vladimir Migashko <migashko@wamba.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef COMPARATORS_COMPARE_MEMBER_HPP
#define COMPARATORS_COMPARE_MEMBER_HPP

# include <vset/comparators/compare.hpp>
#include <fas/functional/member.hpp>
#include <functional>

namespace vset{
  
template<typename V, typename VT, VT V::* m, typename C = std::less<VT> >
struct compare_member: compare< fas::member<V, VT, m>, C > {};

}

#endif
