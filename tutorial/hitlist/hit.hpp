#pragma once

#include <stdint.h>
#include <ctime>

struct hit
{
  uint32_t src_id;
  uint32_t dst_id;
  time_t ts;
};
