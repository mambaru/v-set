//
// Author: Saprykin Dmitry <saprykin.dmitry@gmail.com>, (C) 2014
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>

UNIT(vtree_count, "")
{
  using namespace fas::testing;
  using namespace vset;

  typedef vtree::vtree< vtree::strategy::vtree_fsb_inmem<int, std::less<int>, 3> > int_vtree;

  
  int_vtree tree;
  //({1,2,2,3,6,7,9,15});
  tree.insert(1);
  tree.insert(2);
  tree.insert(2);
  tree.insert(3);
  tree.insert(6);
  tree.insert(7);
  tree.insert(9);
  tree.insert(15);

  
  t << equal<expect, typename int_vtree::size_type>( tree.count(1), static_cast< typename int_vtree::size_type>(1) ) << FAS_TESTING_FILE_LINE;
  t << equal<expect, typename int_vtree::size_type>( tree.count(2), static_cast< typename int_vtree::size_type>(2) ) << FAS_TESTING_FILE_LINE;
  t << equal<expect, typename int_vtree::size_type>( tree.count(-5), static_cast< typename int_vtree::size_type>(0) ) << FAS_TESTING_FILE_LINE;
  t << equal<expect, typename int_vtree::size_type>( tree.count(6), static_cast< typename int_vtree::size_type>(1) ) << FAS_TESTING_FILE_LINE;
  t << equal<expect, typename int_vtree::size_type>( tree.count(11), static_cast< typename int_vtree::size_type>(0) ) << FAS_TESTING_FILE_LINE;
  t << equal<expect, typename int_vtree::size_type>( tree.count(15), static_cast< typename int_vtree::size_type>(1) ) << FAS_TESTING_FILE_LINE;
  t << equal<expect, typename int_vtree::size_type>( tree.count(18), static_cast< typename int_vtree::size_type>(0) ) << FAS_TESTING_FILE_LINE;
  t << equal<expect, typename int_vtree::size_type>( tree.capacity(), static_cast< typename int_vtree::size_type>(18) ) << FAS_TESTING_FILE_LINE;
    
  
  int_vtree tree1;
  tree1.insert(1);
  t << equal<expect, typename int_vtree::size_type>( tree1.capacity(), static_cast< typename int_vtree::size_type>(3) ) << FAS_TESTING_FILE_LINE;

  tree1.insert(544);
  t << equal<expect, typename int_vtree::size_type>( tree1.capacity(), static_cast< typename int_vtree::size_type>(3) ) << FAS_TESTING_FILE_LINE;

  tree1.insert(545);
  t << equal<expect, typename int_vtree::size_type>( tree1.capacity(), static_cast< typename int_vtree::size_type>(3) ) << FAS_TESTING_FILE_LINE;

  tree1.insert(546);
  t << equal<expect, typename int_vtree::size_type>( tree1.capacity(), static_cast< typename int_vtree::size_type>(6) ) << FAS_TESTING_FILE_LINE;
  
  t << nothing;
}

BEGIN_SUITE(vtree_count_suite, "")
  ADD_UNIT(vtree_count)
END_SUITE(vtree_count_suite)
