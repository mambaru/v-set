#include <vset/multiset.hpp>
#include <vset/allocators/mmap_allocator.hpp>
#include <fas/xtime.hpp>
#include <iostream>

#include "config.hpp"
using namespace vset;

typedef vset::multiset< int, std::less<int>, mmap_allocator<CHUNK_SIZE> > storage_type;

int main()
{
  storage_type stg;
  stg.get_allocator().memory().buffer().open("./vset.bin");
  stg.clear();
  //stg.get_allocator().memory().buffer().clear();
  stg.get_allocator().memory().buffer().reserve(MAX_COUNT*8*2);

  fas::nanospan minspan(fas::nanospan::xmax, fas::nanospan::xmax);
  fas::nanospan start = fas::process_nanotime();
  for (int i = 0; i < MAX_COUNT; ++i)
  {
    stg.insert( rand() );
  }
  fas::nanospan finish = fas::process_nanotime();

  std::cout << "init time: " << (finish - start).to_double() << std::endl;
  std::cout << "init rate: " << fas::rate(finish - start)*MAX_COUNT << std::endl;

  start = fas::process_nanotime();
  fas::nanospan start2 = start;
  for (int i = 0; i < MAX_COUNT; ++i)
  {
    stg.find( rand() );
    if (i % MIN_COUNT == 0)
    {
      finish = fas::process_nanotime();
      fas::nanospan tmp = finish - start2;
      if (i != 0 && tmp < minspan)
      {
        minspan = tmp;
      }

#ifdef SHOW_PROCESS
        std::cout << "find time (" << i << "):" << (finish - start).to_double() << std::endl;
        std::cout << "find rate (" << i << "):" <<fas::rate(finish - start)*MAX_COUNT << std::endl;
        std::cout << "find time (" << MIN_COUNT << "):" << (tmp).to_double() << std::endl;
        std::cout << "find rate (" << MIN_COUNT << "):" <<fas::rate(tmp)*MAX_COUNT << std::endl;
#endif
      start2 = fas::process_nanotime();
    }
  }
  finish = fas::process_nanotime();

  std::cout << "---------------------------------" << std::endl;
  std::cout << "final find time (" << MAX_COUNT << "):" << (finish - start).to_double() << std::endl;
  std::cout << "final find rate (" << MAX_COUNT << "):" <<fas::rate(finish - start)*MAX_COUNT << std::endl;
  std::cout << "min find time (" << MIN_COUNT << "):" << (minspan).to_double() << std::endl;
  std::cout << "min find rate (" << MIN_COUNT << "):" <<fas::rate(minspan)*MIN_COUNT << std::endl;

  std::cout << "DONE" << std::endl;

  std::cin.get();
  return 0;
}
