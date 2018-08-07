// #pragma once 
#include <cstdint>
#include <ctime>
#include <vector>
#include <string>
#include <functional>
#include "hitlist/hit.hpp"

class hitlist
{
  class impl;
public:
  hitlist();
  virtual ~hitlist();
  
  bool open(size_t reserve1, size_t reserve2);
  
  std::string desc() const;
  
  void set_hit(uint32_t src, uint32_t dst, time_t ts);
  
  void get_hits( std::vector<hit>& hits, uint32_t id, size_t offset, size_t limit) const;
  
  size_t delete_user(uint32_t id);
  
  size_t remove_outdated(time_t ts);
  
  // сколько просмотрел пользователь с идентификатором id
  size_t src_count(uint32_t id) const;
  
  // сколько было просмотров у пользователя с идентификатором id
  size_t dst_count(uint32_t id) const;
  
  // сколько просмотров было позже ts
  size_t outdated_count(time_t ts) const;
  
  size_t size() const;
  
  size_t capacity() const;
  
  void for_each(std::function<void(hit h)> handler);
  
private:
  
  impl* _impl;
  
};

