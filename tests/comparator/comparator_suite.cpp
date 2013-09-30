//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <vset/multiset.hpp>
#include <fas/testing.hpp>
#include <fas/type_list.hpp>
#include <fas/typemanip.hpp>
#include <set>

#include <vset/comparators/compare.hpp>
#include <vset/comparators/compare_member.hpp>
#include <vset/comparators/compare_list.hpp>
#include <vset/comparators/pointer_compare.hpp>

struct data
{
  int data1;
  int data2;
  int data3;
};

struct cmp_data
{
  cmp_data() {}

  bool operator()(const data& l, const data& r) const
  {
    return l.data1 < r.data1 ||
    ( ! ( r.data1 < l.data1 ) && l.data2 > r.data2 ) ||
    ( ! ( r.data1 < l.data1 ) && ! ( r.data2 > l.data2 ) && l.data3 < r.data3 );
    return false;
  }
};

struct access_stub
{
  template<typename T>
  const T& operator()(const T& t) const
  {
    return t;  
  }
};



template<typename C>
bool test(C c)
{
  bool flag = true;
  flag &= !c( data{1,2,3}, data{1,2,3}) && !c( data{1,2,3}, data{1,2,3} );
  flag &= c( data{1,2,3}, data{2,3,4} ) && !c( data{2,3,4}, data{1,2,3} );
  flag &= c( data{2,3,3}, data{2,2,2} ) && !c( data{2,2,2}, data{2,3,3} );
  flag &= c( data{2,5,4}, data{2,3,4} ) && !c( data{2,3,4}, data{2,5,4} );
  flag &= c( data{2,3,2}, data{2,3,3} ) && !c( data{2,3,3}, data{2,3,2} );
  return flag;
}

typedef fas::type_list_n<
  vset::compare< fas::member<data, int, &data::data1>, std::less<int> >,
  vset::compare< fas::member<data, int, &data::data2>, std::greater<int> >,
  vset::compare< fas::member<data, int, &data::data3>, std::less<int> >
>::type compare1_list;

typedef fas::type_list_n<
  vset::compare_member< data, int, &data::data1>,
  vset::compare_member< data, int, &data::data2, std::greater<int> >,
  vset::compare_member< data, int, &data::data3, std::less<int> >
>::type compare2_list;


struct cmp_data2: vset::compare_list<compare1_list> {};
struct cmp_data3: vset::compare_list<compare2_list> {};

UNIT(comparator_basic, "")
{
  using namespace fas::testing;
  using namespace vset;

  t << is_true<expect>( test(cmp_data() ) ) << FAS_TESTING_FILE_LINE;
  t << is_true<expect>( test(cmp_data2() ) ) << FAS_TESTING_FILE_LINE;
  t << is_true<expect>( test(cmp_data3() ) ) << FAS_TESTING_FILE_LINE;

  t << nothing();
}

UNIT(comparator_set, "")
{
  using namespace fas::testing;
  using namespace vset;
  std::set<data, cmp_data> set1;
  std::set<data, cmp_data2> set2;
  std::set<data*, vset::pointer_compare<cmp_data2> > set3;
  set3.insert(new data{ 1,2,3 } );
  std::set<int*, vset::pointer_compare<std::less<int> > > set4;
  int d[]={1,2,3};
  set4.insert(d);
  set4.insert(d+1);
  set4.insert(d+2);
  //t << is_true<expect>( test(cmp_data() ) ) << FAS_TESTING_FILE_LINE;
  //t << is_true<expect>( test(cmp_data2() ) ) << FAS_TESTING_FILE_LINE;

  t << nothing();
}

BEGIN_SUITE(comparator_suite, "")
  ADD_UNIT(comparator_basic)
  ADD_UNIT(comparator_set)
END_SUITE(comparator_suite)
