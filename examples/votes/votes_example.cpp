#include "votes.hpp"
#include <iostream>

int main()
{
  votes v;
  v.open("./");

  for (id_t i=100; i < 1000; ++i)
  {
    if ( !v.add_content(1, i, 1, 1, 1, 1) )
      std::cout << "Контент существует: " << i << std::endl;
  }

  // Голосуем
  for (id_t i=0; i < 1000; ++i)
  {
    
  }
  return 0;
}