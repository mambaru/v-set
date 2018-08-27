//
// Author: Vladimir Migashko <migashko@wamba.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef COMPARATORS_COMPARE_LIST_HPP
#define COMPARATORS_COMPARE_LIST_HPP

#include <fas/type_list/type_list.hpp>
#include <fas/type_list/type_list_n.hpp>
#include <fas/type_list/empty_list.hpp>
#include <fas/type_list/head.hpp>
#include <fas/type_list/tail.hpp>
#include <fas/type_list/erase_c.hpp>
#include <fas/type_list/push_back.hpp>

namespace vset{

template<typename CompareList>
struct compare_list_impl;

template<typename CompareList>
struct compare_list2_impl;


#ifdef __GXX_EXPERIMENTAL_CXX0X__

/**
 * @brief Создает композитный компаратор на основе списка компараторов
 * @tparam Args... (c++11) список компараторов, например vset::compare_member
 * @details может использоваться для определения компаратора сложных структур.
 */
template<typename... Args>
struct compare_list: compare_list_impl< typename fas::type_list_n<Args...>::type > {};

/**
 * @brief Создает композитный компаратор на основе списка компараторов 
 * @tparam Args... (c++11) список компараторов, например vset::compare_member 
 * @details может использоваться для определения компаратора сложных структур.
 */
template<typename... Args>
struct compare_list2: compare_list2_impl< typename fas::type_list_n<Args...>::type > {};

#else

/**
 * @brief Создает композитный компаратор на основе списка компараторов 
 * @tparam CompareList (c++03) список компараторов (fas::type_list), например из vset::compare_member
 * @details может использоваться для определения компаратора сложных структур.
 */
template<typename CompareList>
struct compare_list: compare_list_impl< CompareList > {};

/**
 * @brief Создает композитный компаратор на основе списка компараторов 
 * @tparam CompareList (c++03) список компараторов (fas::type_list), например из vset::compare_member
 * @details может использоваться для определения компаратора сложных структур.
 */
template<typename CompareList>
struct compare_list2: compare_list2_impl< CompareList > {};

#endif
  
template<typename CompareList>
struct compare_list_impl
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

    return head()(l, r) 
      ? true
      : head()(r, l)
        ? false
        : _(tail(), l, r);
  }

  template< typename D>
  static bool _( fas::empty_list, const D& , const D& )
  {
    return false;
  }
};

template<typename CompareList>
struct compare_list2_impl
{
  template<typename D>
  bool operator()(const D& l, const D& r) const
  {
    return _1_( fas::empty_list(), CompareList(), l, r);
  }

  template< typename L, typename R,  typename D>
  static bool _1_( L, R, const D& l, const D& r) 
  {
    typedef typename fas::push_back< typename fas::head<R>::type, L >::type left;
    typedef typename fas::erase_c< 0, R >::type right;
    return _2_( left(), l, r) || _1_( left(), right(), l, r );
  }

  template< typename L, typename D>
  static bool _1_( L, fas::empty_list, const D& , const D& ) 
  {
    return false;
  }

  template< typename H, typename T, typename D>
  static bool _2_( fas::type_list<H, T>, const D& l, const D& r) 
  {
    return !H()(r, l) && _2_( T(), l, r);
  }

  template< typename H, typename D>
  static bool _2_( fas::type_list<H, fas::empty_list>, const D& l, const D& r) 
  {
    return H()(l, r);
  }
};

}

#endif

