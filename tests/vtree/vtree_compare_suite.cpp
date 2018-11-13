//
// Author: Saprykin Dmitry <saprykin.dmitry@gmail.com>, (C) 2014
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>
#include <vset/compare.hpp>

typedef vset::vtree::vtree< vset::vtree::strategy::vtree_fsb_inmem<int, std::greater<int>, 3> > int_vtree;

UNIT(vtree_compare1, "")
{
  using namespace fas::testing;
  using namespace vset;

  {
    int_vtree tree1, tree2;
    for (int i = 1; i < 4; ++i)
    {
      tree1.insert(i);
      tree2.insert(4-i);
    }
    t << is_true<expect>(tree1 == tree2);
    t << is_true<expect>(tree2 == tree1);
  }
}

UNIT(vtree_compare2, "")
{
  using namespace fas::testing;
  using namespace vset;

  {
    int_vtree tree1, tree2;
    for (int i = 1; i < 4; ++i)
    {
      tree1.insert(2);
      tree2.insert(2);
    }
    tree2.insert(2);
    t << is_false<expect>(tree1 == tree2);
    t << is_false<expect>(tree2 == tree1);
  }

}

UNIT(vtree_compare3, "")
{
  using namespace fas::testing;
  using namespace vset;

  {
    int_vtree tree1, tree2;
    tree1.insert(2); tree2.insert(2);
    tree1.insert(2); tree2.insert(1);
    tree1.insert(2); tree2.insert(2);
    t << is_false<expect>(tree1 == tree2);
  }

}

UNIT(vtree_compare4, "")
{
  using namespace fas::testing;
  using namespace vset;

  {
    int_vtree tree1, tree2;
    tree1.insert(2); tree2.insert(2);
    tree1.insert(2); tree2.insert(2);
    tree1.insert(2); tree2.insert(2);
    t << is_false<expect>(tree1 < tree2);
    t << is_false<expect>(tree2 < tree1);
  }
}

UNIT(vtree_compare5, "")
{
  using namespace fas::testing;
  using namespace vset;

  {
    int_vtree tree1, tree2;
    tree1.insert(2); tree2.insert(2);
    tree1.insert(2); tree2.insert(1);
    tree1.insert(2); tree2.insert(2);
    tree1.insert(3);

    t << is_true<expect>(tree1 < tree2);
  }
}

UNIT(vtree_compare6, "")
{
  using namespace fas::testing;
  using namespace vset;
  {
    int_vtree tree1, tree2;
    tree1.insert(2); tree2.insert(2);
    tree1.insert(2); tree2.insert(2);
    tree1.insert(2); tree2.insert(55);
    t << is_true<expect>(tree2 < tree1);
  }
}

namespace {
  struct foo
  {
    int a;
    int b;
  };
  
  struct foo_compare: vset::compare_list< fas::type_list_n<
    vset::compare_member< foo, int, &foo::a, std::less<int> >,
    vset::compare_member< foo, int, &foo::b, std::greater<int> >
  >::type >{};
  
  typedef vset::vtree::vtree< vset::vtree::strategy::vtree_fsb_inmem<foo, foo_compare, 3> > foo_vtree;
}

UNIT(vtree_compare7, "")
{
  using namespace fas::testing;
  using namespace vset;
  {
    foo f;
    foo_vtree tree1, tree2;
    f.a = 1; f.b = 1; 
    tree1.insert(f); 
    f.a = 1; f.b = 1; 
    tree1.insert(f); 
    f.a = 1; f.b = 1; 
    tree1.insert(f);
    f.a = 1; f.b = 1; 
    tree2.insert(f);
    f.a = 1; f.b = 1; 
    tree2.insert(f);
    f.a = 1; f.b = 2; 
    tree2.insert(f);
    
    t << is_true<expect>(tree2 < tree1);
  }
}

BEGIN_SUITE(vtree_compare_suite, "")
  ADD_UNIT(vtree_compare1)
  ADD_UNIT(vtree_compare2)
  ADD_UNIT(vtree_compare3)
  ADD_UNIT(vtree_compare4)
  ADD_UNIT(vtree_compare5)
  ADD_UNIT(vtree_compare6)
  ADD_UNIT(vtree_compare7)
END_SUITE(vtree_compare_suite)
