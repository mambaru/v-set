//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include "multiset_test_impl.hpp"

UNIT(multiset_mmap_cmp, "")
{
  using namespace fas::testing;
  t << is_true<assert>( multiset_test() ) << FAS_TESTING_FILE_LINE;
  t << nothing();
}

BEGIN_SUITE(multiset_mmap_cmp_suite, "")
  ADD_UNIT(multiset_mmap_cmp)
END_SUITE(multiset_mmap_cmp_suite)
