#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>
#include <fas/xtime.hpp>
#include <iostream>

const int MAX_COUNT = 1024*1024*10;
using namespace vset;

typedef vtree::vtree< vtree::strategy::vtree_fsb_mmap<int, std::less<int>, 512-4*4> > vtree_int;

int main()
{
  vtree_int vtr;
  vtr.get_allocator().memory().buffer().open("./vset.bin");
  vtr.get_allocator().memory().buffer().clear();
  vtr.get_allocator().memory().buffer().reserve(MAX_COUNT*8*2);

  fas::nanospan start = fas::process_nanotime();
  for (int i = 0; i < MAX_COUNT; ++i)
  {
    vtr.insert( rand() );
  }
  fas::nanospan finish = fas::process_nanotime();

  std::cout << (finish - start).to_double() << std::endl;
  std::cout << fas::rate(finish - start)*MAX_COUNT << std::endl;

  start = fas::process_nanotime();
  for (int i = 0; i < MAX_COUNT; ++i)
  {
    vtr.find( rand() );
  }
  finish = fas::process_nanotime();

  std::cout << (finish - start).to_double() << std::endl;
  std::cout << fas::rate(finish - start)*MAX_COUNT << std::endl;

  std::cin.get();
  return 0;
}
