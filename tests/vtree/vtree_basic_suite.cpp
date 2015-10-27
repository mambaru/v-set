//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>

UNIT(vtree_create_filesync, "")
{
  using namespace fas::testing;
  using namespace vset;

  
  typedef vtree::vtree< vtree::strategy::vtree_fsb_mmap<char, std::less<char>, 3> > int_vtree;
  int_vtree tree;
  
  unlink("vtree.bin");
  tree.get_allocator().memory().buffer().open("vtree.bin");
  
  
  tree.lower_bound('B');
  tree.upper_bound('B');
  
  for (int i = 0; i < 5 ; ++i)
  {
    tree.insert('A' + i % 10);
  }

  
  int_vtree::iterator itr = tree.begin();
  for (int i = 0; itr != tree.end(); ++itr, ++i)
  {
    t << equal<expect, char>(*itr, 'A' + i % 10);
  }

  // g++-4.6 internal compiler error
  //tree.get_allocator().memory().buffer().sync();
  //t << equal<expect, size_t>(tree.get_container().size(), 3);
  //t << equal<expect, size_t>(tree.size(), 5);
  //tree.get_allocator().memory().buffer().close();
  
  t << nothing;
}

BEGIN_SUITE(vtree_basic_suite, "")
  ADD_UNIT(vtree_create_filesync)
END_SUITE(vtree_basic_suite)
