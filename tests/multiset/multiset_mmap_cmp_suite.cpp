//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/memory/manager.hpp>
#include <vset/allocators/allocator.hpp>
#include <vset/comparators/offset_compare.hpp>
#include <vset/comparators/compare_list.hpp>
#include <vset/comparators/compare_member.hpp>
#include <vset/multiset.hpp>
#include "multiset_test_impl.hpp"


UNIT(multiset_mmap_cmp, "")
{
  using namespace fas::testing;
  t << flush;
  t << is_true<assert>( multiset_test() ) << FAS_FL;
}

struct item
{
  int item1;
  int item2;
  item(): item1(0), item2(){}
  item(int i1, int i2): item1(i1), item2(i2){}
};

typedef vset::compare_list< fas::type_list_n<
    vset::compare_member<item, int, &item::item1>,
    vset::compare_member<item, int, &item::item2, std::greater<int> >
  >::type
> item_cmp;
struct item_cmp_t: item_cmp{};

#ifdef NDEBUG
#define TEST_COUNT 50
#else
#define TEST_COUNT 3
#endif

typedef size_t offset_t;
typedef vset::memory::manager< ::vset::memory::strategy::fsb_inmem<item, ::vset::memory::fsb::aspect_offset> > int_data;
struct int_data_t: int_data{};
typedef vset::offset_compare< offset_t, int_data_t, item_cmp_t > offset_greater;
typedef vset::multiset< offset_t, offset_greater, vset::allocator<2> > int_index;

UNIT(multiset_mmap_lu, "")
{
  using namespace fas::testing;
  t << nothing;

  int_data data;
  int_index index( offset_greater( data.end() ) );
  int_data::pointer val = data.allocate(1);
  for (int i = 0; i < TEST_COUNT; ++i)
    for (int j = 0; j < TEST_COUNT; ++j)
  {
    int_data::pointer ins = data.allocate(1);
    *ins = item(i, -j);
    index.insert( ins.get_offset() );
  }

  int_data::pointer ptr = data.end();
  for (int i = 0; i < TEST_COUNT; ++i)
    for (int j = 0; j < TEST_COUNT; ++j)
  {
    *val = item(i, -j);
    
    int_index::iterator lower = index.lower_bound( val.get_offset() );
    int_index::iterator upper = index.upper_bound( val.get_offset() );
    t << equal<expect, size_t>( std::distance(lower, upper), 1 ) << FAS_FL;
    --upper;
    ptr.set_offset(*lower);
    item a = *ptr;
    ptr.set_offset(*upper);
    item b = *ptr;
    t << equal<expect>(a.item1, b.item1) << FAS_FL;
    t << equal<expect>(a.item2, b.item2) << FAS_FL;
  }
  
  /*
  int_index::iterator beg = index.begin();
  for ( ; beg != index.end(); ++beg) 
  {
    ptr.set_offset(*beg);
    item a = *ptr;
    t << message("a= ")  << a.item1 << ", " <<  a.item2;
  }*/
}

BEGIN_SUITE(multiset_mmap_cmp_suite, "")
  ADD_UNIT(multiset_mmap_cmp)
  ADD_UNIT(multiset_mmap_lu)
END_SUITE(multiset_mmap_cmp_suite)
