//#pragma once 
#include <cstdint>
#include <ctime>
#include <vector>
#include <string>
#include "hitlist/hit.hpp"

class hitlist
{
  class impl;
public:
  hitlist();
  virtual ~hitlist();
  
  void open();
  
  std::string desc() const;
  
  void set_hit(uint32_t src, uint32_t dst, time_t ts);
  
  void get_hits( std::vector<hit>& hits, uint32_t id, size_t offset, size_t limit) const;
  
  bool delete_user(uint32_t id);
  
  size_t remove_outdated(time_t ts);
  
  total get_total(uint32_t) const;
  
  size_t size() const;
  
  size_t capacity() const;
  
private:
  
  impl* _impl;
  
};

