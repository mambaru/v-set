//
// Author: Dmitry Saprykin <saprykin.dmitry@gmail.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#include <vset/multiset.hpp>
#include <fas/testing.hpp>
#include <set>

UNIT(defrag, "")
{
  
  using namespace fas::testing;
  using namespace vset;

  typedef multiset<int, std::greater<int> > multiset_type;
  multiset_type int_set;

  std::set<int> etalon;

  for(unsigned int i = 0; i < 10 * 1024; ++i) {
    int_set.insert(i);
    etalon.insert(i);
  }

  std::cout << std::endl;
  for(unsigned int i = 0; i < 220; ++i)
  {
    size_t size = 0;
    size_t size_max = 1;
    typedef multiset_type::container_type container_type;
    container_type& container = int_set.get_container();
    for (  container_type::iterator it = container.begin(); it != container.end(); ++it)
    {
      typename container_type::value_type::second_type second = (*it).second;

      size += second->size();
      size_max += second->capacity();
    }

    t << equal<expect, size_t>( int_set.size(), etalon.size()) << FAS_TESTING_FILE_LINE;
    t << is_true<expect>( (size * 100)/size_max > 25) << FAS_TESTING_FILE_LINE;;

    size_t skip = 0;
    container_type::iterator it = container.begin();
    while( it != container.end() )
    {
      size_t skipped = 0;
      for (; skipped < skip && it != container.end(); ++it, ++skipped);
      if( it != container.end() )
      {
        int erase_item = (*it).second->back();
        int_set.erase(erase_item);
        etalon.erase(erase_item);
        it = container.begin();
        skip++;
      }
    }
  }

  t << nothing();
}

BEGIN_SUITE(defrag_suite, "")
  ADD_UNIT(defrag)
END_SUITE(defrag_suite)
