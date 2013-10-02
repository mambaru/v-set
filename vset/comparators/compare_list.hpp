//
// Author: Vladimir Migashko <migashko@wamba.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#pragma once

#include <fas/type_list/type_list.hpp>
#include <fas/type_list/empty_list.hpp>
#include <fas/type_list/head.hpp>
#include <fas/type_list/tail.hpp>
#include <fas/type_list/erase_c.hpp>
#include <fas/type_list/push_back.hpp>

namespace vset{
  
template<typename CompareList>
struct compare_list
{
  template<typename D>
  bool operator()(const D& l, const D& r) const
  {
    return _( CompareList(), l, r);
  }

  template< typename L, typename D>
  bool _( L, const D& l, const D& r) const
  {
    typedef typename fas::head<L>::type head;
    typedef typename fas::tail<L>::type tail;

    if ( head()(l, r) )
      return true;

    if ( head()(r, l) )
      return false;

    return _(tail(), l, r);
  }

  template< typename D>
  bool _( fas::empty_list, const D& , const D& ) const
  {
    return false;
  }
};

template<typename CompareList>
struct compare_list2
{
  template<typename D>
  bool operator()(const D& l, const D& r) const
  {
    return _1_( fas::empty_list(), CompareList(), l, r);
  }

  template< typename L, typename R,  typename D>
  bool _1_( L, R, const D& l, const D& r) const
  {
    typedef typename fas::push_back< typename fas::head<R>::type, L >::type left;
    typedef typename fas::erase_c< 0, R >::type right;
    return _2_( left(), l, r)
            || _1_( left(), right(), l, r );
  }

  template< typename L, typename D>
  bool _1_( L, fas::empty_list, const D& , const D& ) const
  {
    return false;
  }

  template< typename H, typename T, typename D>
  bool _2_( fas::type_list<H, T>, const D& l, const D& r) const
  {
    return !H()(r, l) && _2_( T(), l, r);
  }

  template< typename H, typename D>
  bool _2_( fas::type_list<H, fas::empty_list>, const D& l, const D& r) const
  {
    return H()(l, r);
  }
};


}
