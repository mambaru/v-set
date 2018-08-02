#pragma once

#include <stdint.h>
#include <ctime>

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
};

struct total
{
  size_t src_hits = 0;
  size_t dst_hits = 0;
};
