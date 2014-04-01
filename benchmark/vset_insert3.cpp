#include <vset/vtree/vtree.hpp>
#include <stx/btree_multimap.h>
#include <fas/xtime.hpp>
#include <iostream>

#include <vset/multiset.hpp>
#include <vset/allocators/allocator.hpp>
#include <fas/xtime.hpp>
#include <iostream>

#include "config.hpp"
using namespace vset;

//typedef vset::multiset< int, std::less<int>, allocator<CHUNK_SIZE> > storage_type;
//typedef vset::multiset< int, std::less<int>, std::allocator<int> > storage_type;
// typedef vtree::vtree< vtree::aspect<int, std::less<int>, 512-4*4> > storage_type;

template <int _innerslots, int _leafslots>
struct btree_traits_speed
{
    static const bool   selfverify = false;
    static const bool   debug = false;

    static const int    leafslots = _innerslots;
    static const int    innerslots = _leafslots;
};

struct ad_container
{
  template<typename K, typename V, typename C>
  struct apply
  {
    //typedef std::multimap<K, V> type;
    typedef stx::btree_multimap<K, V, C,
                            struct btree_traits_speed<8, 512> > type;
  };
};

struct aspect: fas::aspect< fas::type_list_n<
    fas::advice< vset::vtree::_container_, ad_container >,
    vtree::aspect2<int, std::less<int>, 512>
>::type >{};

//typedef vtree::vtree< vtree::aspect2<int, std::less<int>, 128> > vtree_int;
typedef vtree::vtree< aspect > storage_type;


int main()
{
  storage_type stg;
  /*
  stg.get_allocator().memory().buffer().open("./vset.bin");
  stg.get_allocator().memory().buffer().clear();
  stg.get_allocator().memory().buffer().reserve(MAX_COUNT*8*2);
  */

  fas::nanospan minspan(fas::nanospan::xmax, fas::nanospan::xmax);
  fas::nanospan start = fas::process_nanotime();
  for (int i=0; i < MAX_COUNT; ++i)
    stg.insert( rand() );
  fas::nanospan finish = fas::process_nanotime();

  std::cout << "init time: " << (finish - start).to_double() << std::endl;
  std::cout << "init rate: " << fas::rate(finish - start)*MAX_COUNT << std::endl;

  start = fas::process_nanotime();
  fas::nanospan start2 = start;
  for (int i=0; i < MAX_COUNT; ++i)
  {
    stg.find( rand() );
    if (i%MIN_COUNT==0)
    {
      finish = fas::process_nanotime();
      fas::nanospan tmp = finish - start2;
      if (i!=0 && tmp < minspan)
        minspan = tmp;
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
  // std::cout << (finish - start).to_double() << std::endl;
  // std::cout << fas::rate(finish - start)*MAX_COUNT << std::endl;
  std::cout << "final find time (" << MAX_COUNT << "):" << (finish - start).to_double() << std::endl;
  std::cout << "final find rate (" << MAX_COUNT << "):" <<fas::rate(finish - start)*MAX_COUNT << std::endl;
  std::cout << "min find time (" << MIN_COUNT << "):" << (minspan).to_double() << std::endl;
  std::cout << "min find rate (" << MIN_COUNT << "):" <<fas::rate(minspan)*MIN_COUNT << std::endl;

  std::cout << "DONE" << std::endl;

  std::cin.get();
  return 0;
}
/*
const int MAX_COUNT = 1024*1024*10;
using namespace vset;

/// Traits used for the speed tests, BTREE_DEBUG is not defined.
template <int _innerslots, int _leafslots>
struct btree_traits_speed
{
    static const bool   selfverify = false;
    static const bool   debug = false;

    static const int    leafslots = _innerslots;
    static const int    innerslots = _leafslots;
};


struct ad_container
{
  template<typename K, typename V>
  struct apply
  {
    //typedef std::multimap<K, V> type;
    typedef stx::btree_multimap<K, V, std::less<K>,
                            struct btree_traits_speed<8, 512> > type;
  };
};


struct aspect: fas::aspect< fas::type_list_n<
    fas::advice< vset::vtree::_container_, ad_container >,
    vtree::aspect2<int, std::less<int>, 512>
>::type >{};

//typedef vtree::vtree< vtree::aspect2<int, std::less<int>, 128> > vtree_int;
typedef vtree::vtree< aspect > vtree_int;

int main()
{
  vtree_int vtr;

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
*/