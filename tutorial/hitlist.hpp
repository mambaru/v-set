#pragma once 
#include <cstdint>
#include <ctime>
#include <vector>
#include "hitlist/hit.hpp"

class hitlist
{
  class impl;
public:
  hitlist();
  virtual ~hitlist();
  
  void open();
  
  void set_hit(uint32_t src, uint32_t dst, time_t ts);
  
  size_t get_hits( std::vector<hit>& hits, uint32_t id, size_t offset, size_t limit) const;
  
  size_t size() const;
  
  size_t capacity() const;
  
private:
  
  impl* _impl;
  
};

