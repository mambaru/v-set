//
// Author: Saprykin Dmitry <saprykin.dmitry@gmail.com>, (C) 2014
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>

UNIT(vtree_copy_ctor, "")
{
  using namespace fas::testing;
  using namespace vset;

  //copy constructor is disabled
  /*typedef vtree::vtree< vtree::strategy::vtree_fsb_mmap<char, std::less<char>, 3> > int_vtree;
  int_vtree tree;
  int_vtree tree1(tree);*/

  struct test_type
  {
    char key;
    char value;

    bool operator==(const test_type &other)
    {
      return key == other.key && value == other.value;
    }
  };

  struct cmp_test_type
  {
    bool operator()(const test_type& l, const test_type& r) const
    {
      return l.key < r.key;
    }
  };

  typedef vtree::vtree< vtree::strategy::vtree_fsb_inmem<test_type, cmp_test_type, 3> > int_vtree;

  //copy ctor + initializer list ctor
  int_vtree tree({{1,2},{3,4},{5,6},{7,8}});

  int_vtree tree1(tree);
  tree1.erase(test_type({3,0}));
  tree1.insert(test_type({3,8}));

  auto itr1 = tree1.begin();
  for(auto itr = tree.begin() ;itr != tree.end(); ++itr, ++itr1 )
  {
    if( itr->key == 3 )
    {
      t << equal<expect, char>( itr->value, 4 ) << FAS_TESTING_FILE_LINE;
      t << equal<expect, char>( itr1->value, 8 ) << FAS_TESTING_FILE_LINE;
    }
    else
    {
      t << equal<expect, test_type>( *itr, *itr1 ) << FAS_TESTING_FILE_LINE;
    }
  }

  //move ctor
  int_vtree tree2(std::move(tree1));

  auto itr2 = tree2.begin();
  for(auto itr = tree.begin(); itr != tree.end(); ++itr, ++itr2 )
  {
    if( itr->key == 3 )
    {
      t << equal<expect, char>( itr->value, 4 ) << FAS_TESTING_FILE_LINE;
      t << equal<expect, char>( itr2->value, 8 ) << FAS_TESTING_FILE_LINE;
    }
    else
    {
      t << equal<expect, test_type>( *itr, *itr2 ) << FAS_TESTING_FILE_LINE;
    }
  }

  t << equal<expect, typename int_vtree::size_type>( tree1.size(), static_cast< typename int_vtree::size_type>(0) ) << FAS_TESTING_FILE_LINE;
  t << equal<expect, typename int_vtree::size_type>( tree2.size(), static_cast< typename int_vtree::size_type>(4) ) << FAS_TESTING_FILE_LINE;

  t << nothing();
}

BEGIN_SUITE(vtree_ctor_suite, "")
  ADD_UNIT(vtree_copy_ctor)
END_SUITE(vtree_ctor_suite)
