//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/vtree/array.hpp>
#include <vset/vtree/sorted_array.hpp>



UNIT(test_array, "")
{
  using namespace fas::testing;
  using namespace vset::vtree;

  array<int, 1024> arr;
  t << nothing();
}

UNIT(test_sorted_array, "")
{
  using namespace fas::testing;
  using namespace vset::vtree;

  sorted_array<int, 1024> arr;
  t << nothing();
}

BEGIN_SUITE(array_suite, "")
  ADD_UNIT(test_array)
  ADD_UNIT(test_sorted_array)
END_SUITE(array_suite)
