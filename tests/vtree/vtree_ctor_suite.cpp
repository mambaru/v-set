//
// Author: Saprykin Dmitry <saprykin.dmitry@gmail.com>, (C) 2014
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>

namespace
{
  using namespace vset;
  
  struct test_type
  {
    char key;
    char value;
    test_type(): key(0), value(0) {}
    test_type(char k, char v): key(k), value(v) {}

    bool operator==(const test_type &other) const
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
}
  
UNIT(vtree_copy_ctor, "")
{
  using namespace fas::testing;
  using namespace vset;

  //copy constructor is disabled
  /*typedef vtree::vtree< vtree::strategy::vtree_fsb_mmap<char, std::less<char>, 3> > ctor_int_vtree;
  ctor_int_vtree ctor_tree;
  ctor_int_vtree ctor_tree1(ctor_tree);*/


  //copy ctor + initializer list ctor
  int_vtree tree;
  tree.insert(test_type(1,2));
  tree.insert(test_type(3,4));
  tree.insert(test_type(5,6));
  tree.insert(test_type(7,8));

  int_vtree tree1(tree);
  tree1.erase(test_type(3,0));
  tree1.insert(test_type(3,8));

  typename int_vtree::iterator itr1 = tree1.begin();
  for(typename int_vtree::iterator itr = tree.begin() ;itr != tree.end(); ++itr, ++itr1 )
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
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  int_vtree tree2(std::move(tree1));
#else
  int_vtree tree2(tree1);
  tree1.clear();
#endif

  typename int_vtree::iterator itr2 = tree2.begin();
  for(typename int_vtree::iterator itr = tree.begin(); itr != tree.end(); ++itr, ++itr2 )
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

  t << nothing;
}

BEGIN_SUITE(vtree_ctor_suite, "")
  ADD_UNIT(vtree_copy_ctor)
END_SUITE(vtree_ctor_suite)
