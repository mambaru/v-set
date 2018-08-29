#include <stx/btree_multimap.h>
#include <fas/xtime.hpp>
#include <iostream>

const int MAX_COUNT = 1024*1024*10;

/// Traits used for the speed tests, BTREE_DEBUG is not defined.
template <int _innerslots, int _leafslots>
struct btree_traits_speed
{
  static const bool selfverify = false;
  static const bool debug = false;
  static const int leafslots = _innerslots;
  static const int innerslots = _leafslots;
  static const size_t binsearch_threshold = 256;
};

typedef stx::btree_multimap<int, int, std::less<int>,
                            struct btree_traits_speed<128, 128> > btree_type;
int main()
{
  srand(42);
  btree_type vtr;
  fas::nanospan start = fas::process_nanotime();
  for (int i = 0; i < MAX_COUNT; ++i)
  {
    vtr.insert( std::make_pair(rand(), rand()) );
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
