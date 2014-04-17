//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <vset/multiset.hpp>
#include <vset/allocators/allocator.hpp>
#include <vset/allocators/mmap_allocator.hpp>
#include <vset/allocators/buffer_allocator.hpp>
#include <fas/testing.hpp>

UNIT(multiset_alloc, "")
{
  using namespace fas::testing;
  using namespace vset;

  typedef multiset<int, std::greater<int>, buffer_allocator<512> > multiset_type;
  multiset_type int_set;
  int_set.get_allocator().memory().buffer().reserve(10000000);

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

  int_set.erase(10);

  t << equal<expect, size_t>( int_set.size(), 2) << FAS_TESTING_FILE_LINE;

  bool flag = false;
  try
  {
    int_set.erase(int_set.end());
  }
  catch(const std::out_of_range&)
  {
    flag = true;
  }

  t << is_true<expect>( flag ) << FAS_TESTING_FILE_LINE;

  t << nothing();
}

UNIT(multiset2, "")
{
  using namespace fas::testing;
  using namespace vset;

  // 0,1,2 ; 3,4,5 ; 6,7,8
  typedef multiset<int, std::greater<int>, allocator<3> > multiset_type;
  multiset_type int_set;
  for (int i=0; i < 9; i++)
  {
    int_set.insert(i);
  }

  for (int i=0; i < 9; i++)
  {
    multiset_type::iterator lower = int_set.lower_bound(i);
    t << equal<expect>( *lower, i) << " i=" << i << " " << FAS_TESTING_FILE_LINE;
  }

  for (int i=0; i < 9; i++)
  {
    multiset_type::iterator lower = int_set.upper_bound(i);
    t << equal<expect>( *(--lower), i) << " i=" << i << " " << FAS_TESTING_FILE_LINE;
  }

  for (int i = 0; i < 9; i += 2)
  {
    int_set.erase(i);
  }

  for ( int i=0; i < 9; ++i )
  {
    multiset_type::iterator lower = int_set.lower_bound(i);
    if ( i == 0 && lower == int_set.end() )
      continue;
    t << is_true<assert>( lower != int_set.end() ) << " i=" << i << " " << FAS_TESTING_FILE_LINE;
    if (i%2!=0)
      t << equal<assert>( *lower, i ) << " i=" << i /*<< "," << i */<< " !="<< *lower<< " " << FAS_TESTING_FILE_LINE;
    else
      t << equal<assert>( *lower, i-1 ) << " i=" << i-1<< " !="<< *lower<< " " << FAS_TESTING_FILE_LINE;
  }
}


BEGIN_SUITE(multiset2_suite, "")
  ADD_UNIT(multiset_alloc)
  ADD_UNIT(multiset2)
END_SUITE(multiset2_suite)
