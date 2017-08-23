//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/memory/fsb_mmap.hpp>
#include <vset/memory/fsb_filesync.hpp>
#include <vset/memory/fsb_inmem.hpp>

UNIT(fsb_mmap, "")
{
  using namespace fas::testing;
  using namespace vset;
  memory::fsb_mmap<int> fsb_mmap;
  fsb_mmap.buffer().open("./fsb_mmap.bin");
  fsb_mmap.buffer().clear();
  fsb_mmap.buffer().close();
  t << nothing;
}

UNIT(fsb_filesync, "")
{
  using namespace fas::testing;
  using namespace vset;
  memory::fsb_filesync<int> fsb_filesync;
  fsb_filesync.buffer().open("./fsb_filesync.bin");
  fsb_filesync.buffer().clear();
  fsb_filesync.buffer().close();
  t << nothing;
}

UNIT(fsb_inmem, "")
{
  using namespace fas::testing;
  using namespace vset;
  memory::fsb_inmem<int> fsb_inmem;
  fsb_inmem.allocate(1);
  t << nothing;
}



BEGIN_SUITE(manager_suite, "")
  ADD_UNIT(fsb_mmap)
  ADD_UNIT(fsb_filesync)
END_SUITE(manager_suite)
