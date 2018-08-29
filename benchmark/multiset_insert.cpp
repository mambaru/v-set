/*#include <set>
#include <fas/xtime.hpp>
#include <iostream>

const int MAX_COUNT = 1024*1024*10;

int main()
{
  std::multiset<int> vtr;

  fas::nanospan start = fas::process_nanotime();
  for (int i=0; i < MAX_COUNT; ++i)
    vtr.insert( rand() );
  fas::nanospan finish = fas::process_nanotime();

  std::cout << (finish - start).to_double() << std::endl;
  std::cout << fas::rate(finish - start)*MAX_COUNT << std::endl;

  std::cin.get();
  return 0;
}
*/

#include <set>
#include <fas/xtime.hpp>
#include <iostream>

#include "config.hpp"

typedef std::multiset<int> storage_type;

int main()
{
  storage_type stg;

  fas::nanospan minspan(fas::nanospan::xmax, fas::nanospan::xmax);
  srand(42);
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
  for (int i=0; i < MAX_COUNT; ++i)
  {
    stg.find( rand() );
    if (i%MIN_COUNT==0)
    {
      finish = fas::process_nanotime();
      fas::nanospan tmp = finish - start2;
      if (i!=0 && tmp < minspan)
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
