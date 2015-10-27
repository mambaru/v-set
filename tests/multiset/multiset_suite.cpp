//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <vset/multiset.hpp>
#include <fas/testing.hpp>

UNIT(multiset_basic, "")
{
  using namespace fas::testing;
  using namespace vset;

  typedef multiset<int, std::greater<int> > multiset_type;
  multiset_type int_set;

  int_set.insert(1);
  int_set.insert(1);
  int_set.insert(1);

  int_set.insert(2);
  int_set.insert(2);
  int_set.insert(2);

  t << equal<expect, size_t>( int_set.size(), 6) << FAS_TESTING_FILE_LINE;


  multiset_type::iterator itr = int_set.find(2);

  int_set.erase(itr);

  t << equal<expect, size_t>( int_set.size(), 5) << FAS_TESTING_FILE_LINE;

  int_set.erase(1);

  t << equal<expect, size_t>( int_set.size(), 2) << FAS_TESTING_FILE_LINE;


  t << nothing;
}

BEGIN_SUITE(multiset_suite, "")
  ADD_UNIT(multiset_basic)
END_SUITE(multiset_suite)
