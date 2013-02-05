#include <vset/vtree/vtree.hpp>
#include <stx/btree_multimap.h>
#include <fas/xtime.hpp>
#include <iostream>

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