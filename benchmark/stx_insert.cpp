#include <stx/btree_multiset.h>
#include <fas/xtime.hpp>
#include <iostream>

const int MAX_COUNT = 1024*1024*10;

/// Traits used for the speed tests, BTREE_DEBUG is not defined.
template <int _innerslots, int _leafslots>
struct btree_traits_speed
{
    static const bool   selfverify = false;
    static const bool   debug = false;

    static const int    leafslots = _innerslots;
    static const int    innerslots = _leafslots;
};

typedef stx::btree_multiset<int, std::less<int>,
                            struct btree_traits_speed<128, 128> > btree_type;
int main()
{
  btree_type vtr;

  fas::nanospan start = fas::process_nanotime();
  for (int i=0; i < MAX_COUNT; ++i)
    vtr.insert( rand() );
  fas::nanospan finish = fas::process_nanotime();

  std::cout << (finish - start).to_double() << std::endl;
  std::cout << fas::rate(finish - start)*MAX_COUNT << std::endl;

  start = fas::process_nanotime();
  for (int i=0; i < MAX_COUNT; ++i)
    vtr.find( rand() );
  finish = fas::process_nanotime();

  std::cout << (finish - start).to_double() << std::endl;
  std::cout << fas::rate(finish - start)*MAX_COUNT << std::endl;


  std::cin.get();
  return 0;
}

