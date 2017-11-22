#include <stx/btree_multiset.h>
#include <fas/xtime.hpp>
#include <iostream>

#include <set>
#include <fas/xtime.hpp>
#include <iostream>

#include "config.hpp"

template <int _innerslots, int _leafslots>
struct btree_traits_speed
{
  static const bool selfverify = false;
  static const bool debug = false;
  static const int leafslots = _innerslots;
  static const int innerslots = _leafslots;
};

typedef stx::btree_multiset<int, std::less<int>, struct btree_traits_speed<CHUNK_SIZE, CHUNK_SIZE> > storage_type;

int main()
{
  storage_type stg;

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

      if ( SHOW_PROCESS )
      {
        std::cout << "find time (" << i << "):" << (finish - start).to_double() << std::endl;
        std::cout << "find rate (" << i << "):" <<fas::rate(finish - start)*MAX_COUNT << std::endl;
        std::cout << "find time (" << MIN_COUNT << "):" << (tmp).to_double() << std::endl;
        std::cout << "find rate (" << MIN_COUNT << "):" <<fas::rate(tmp)*MAX_COUNT << std::endl;
      }
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
