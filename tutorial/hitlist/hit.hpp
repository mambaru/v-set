#pragma once

#include <stdint.h>
#include <ctime>
#include <limits>

struct hit
{
  uint32_t src_id = 0;
  uint32_t dst_id = 0;
  time_t ts  = 0;
  
  static hit make(uint32_t src_id, uint32_t dst_id, time_t ts)
  {
    hit h;
    h.src_id = src_id;
    h.dst_id = dst_id;
    h.ts = ts;
    return h;
  }
  
  static hit make_lower_src(uint32_t src_id)
  {
    return hit::make(src_id, 0, std::numeric_limits<time_t>::max());
  }

  static hit make_upper_src(uint32_t src_id)
  {
    return hit::make(src_id, std::numeric_limits<uint32_t>::max(), 0);
  }

  static hit make_lower_dst(uint32_t dst_id)
  {
    return hit::make(0, dst_id, std::numeric_limits<time_t>::max());
  }

  static hit make_upper_dst(uint32_t dst_id)
  {
    return hit::make(std::numeric_limits<uint32_t>::max(), dst_id, 0);
  }
};

template<typename T>
struct hit_idx_t: hit
{
  T target;
};
