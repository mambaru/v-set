//
// Author: Saprykin Dmitry <saprykin.dmitry@gmail.com>, (C) 2014
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>

UNIT(vtree_compare, "")
{
  using namespace fas::testing;
  using namespace vset;

  typedef vtree::vtree< vtree::strategy::vtree_fsb_inmem<int, std::less<int>, 3> > int_vtree;

  {
    int_vtree tree1({2,2,2}), tree2({2,2,2});
    t << is_true<expect>(tree1 == tree2);
    t << is_true<expect>(tree2 == tree1);
  }

  {
    int_vtree tree1({2,2,2}), tree2({2,2,2,2});
    t << is_false<expect>(tree1 == tree2);
    t << is_false<expect>(tree2 == tree1);
  }

  {
    int_vtree tree1({2,2,2}), tree2({2,1,2});
    t << is_false<expect>(tree1 == tree2);
  }

  {
    int_vtree tree1({2,2,2}), tree2({2,2,2});
    t << is_false<expect>(tree1 < tree2);
    t << is_false<expect>(tree2 < tree1);
  }

  {
    int_vtree tree1({2,2,2,3}), tree2({2,2,2});
    t << is_false<expect>(tree1 < tree2);
  }

  {
    int_vtree tree1({2,2,2}), tree2({2,2,55});
    t << is_true<expect>(tree1 < tree2);
  }

  t << nothing();
}

BEGIN_SUITE(vtree_compare_suite, "")
  ADD_UNIT(vtree_compare)
END_SUITE(vtree_compare_suite)
