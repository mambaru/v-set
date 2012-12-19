//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VTREE_ASPECT_ASPECT_HPP
#define VSET_VTREE_ASPECT_ASPECT_HPP

#include <vset/vtree/aspect/aspect_insert.hpp>
#include <vset/vtree/aspect/tags.hpp>
#include <vset/sorted_array.hpp>

#include <vset/memory/provider.hpp>
#include <vset/memory/allocator.hpp>
#include <vset/memory/fsb/aspect.hpp>
#include <fas/aop.hpp>
#include <fas/mp.hpp>

#include <fas/aop/provider.hpp>

namespace vset{ namespace vtree{

struct ad_default_allocator_builder
{
  template<typename T>
  struct build
  {
    typedef typename T::aspect::template advice_cast<_array_type_>::type array_type;
    typedef std::allocator<array_type> type;
  };

  template<typename T>
  typename build<T>::type operator()(T& ) const
  {
    return build<T>::type();
  }
};

struct ad_persistent_allocator_builder
{
  template<typename T>
  struct build
  {
    typedef memory::provider< T > provider;
    typedef memory::allocator< provider > type;
  };

  template<typename T>
  typename build<T>::type operator()(T& t) const
  {
    return build<T>::type(&t);
  }
};

template<typename Tg>
struct ad_advanced_builder
{
  template<typename T>
  struct build
  {
    typedef typename T::aspect::template advice_cast< Tg >::type wrap;
    typedef typename fas::apply<typename wrap::type, T>::type type;
  };

  template<typename T>
  typename build<T>::type operator()(T& t) const
  {
    return build<T>::type(&t);
  }
};

template<typename Tg>
struct ad_simple_builder
{
  template<typename T>
  struct build
  {
    typedef typename T::aspect::template advice_cast< Tg >::type type;
  };

  template<typename T>
  typename build<T>::type operator()(T& ) const
  {
    return build<T>::type();
  }
};

struct ad_restore
{
  template<typename T>
  void operator()(T& t)
  {
    typedef typename T::allocator_type::memory_type::pointer pointer;
    pointer beg = t.get_allocator().memory().begin();
    pointer end = t.get_allocator().memory().end();
    for (;beg!=end;++beg)
    {
      t.get_aspect().template get<_insert_to_container_>()(t, std::make_pair( beg->front(), beg->back() ), beg);
      t.get_aspect().template get<_size_>() += beg->size();
    }
  }
};


template<typename V, typename Compare = std::less<V> >
struct value_aspect: fas::aspect< typename fas::type_list_n<
  fas::type_advice<_value_type_, V >,
  fas::alias<_key_type_, _value_type_>,
  fas::value_advice<_compare_, Compare >,
  fas::alias< _key_compare_, _compare_ >,
  fas::alias< _value_compare_, _compare_ >
  
  /*
  fas::type_advice<_compare_, fas::w< std::less< fas::_ > > >,
  fas::alias<_key_compare_, _compare_ >,
  fas::alias<_value_compare_, _compare_ >,
  fas::type_advice<_key_compare_builder_, ad_simple_builder<_key_compare_> >,
  fas::type_advice<_value_compare_builder_, ad_simple_builder<_value_compare_> >
  */
  
>::type> {};

/*
template<typename V, typename Compare = std::less<V>, int N = 1024 >
struct memory_aspect: fas::aspect< typename fas::type_list_n<
  fas::type_advice<_array_type_, sorted_array< V, N, std::less<V> > >,
  fas::advice<_allocator_, ad_persistent_allocator_builder>,
  memory::fsb::aspect< sorted_array< V, N, std::less<V> > >
>::type> {};
*/


template<typename Array >
struct memory_aspect: fas::aspect< typename fas::type_list_n<
  fas::type_advice<_array_type_, Array >,
  fas::advice<_allocator_, fas::provider< fas::w< memory::allocator< memory::provider< fas::_ > > > > >,
  /*
  fas::type_advice<_allocator_, fas::w< memory::allocator< memory::provider< fas::_ > > > >,
  fas::advice<_allocator_builder_, ad_advanced_builder<_allocator_> >,
  */

  fas::value_advice< _size_, size_t>,
  /*fas::value_advice< _container_,
    std::multimap<
      std::pair<typename Array::value_type, typename Array::value_type>,
      Array*
      // TODO: tree compare
    >
  >,*/
  
  memory::fsb::aspect<Array >
>::type> {};


template<typename V, typename Compare = std::less<V>, int N = 1024 >
struct aspect: fas::aspect_merge<
  value_aspect<V, Compare>,
  memory_aspect< sorted_array< V, N, std::less<V> > >,
  fas::advice< _restore_, ad_restore >,
  fas::group< buffer::persistent::_after_open_, _restore_ >,
  aspect_insert
>::type {};

}}

#endif
