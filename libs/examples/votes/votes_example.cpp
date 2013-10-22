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

  std::vector<content> result;
  if ( !v.get_content_for_rating(1, 100, 100, result) )
    std::cout << "not found" << std::endl;
  std::cout << result.size() << std::endl;
  for (auto c: result)
  {
    std::cout << "content: " << c.content_id << std::endl;
  }
  return 0;
}