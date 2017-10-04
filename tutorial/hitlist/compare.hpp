#pragma once

#include <hitlist/hit.hpp>
#include <vset/comparators/compare_list.hpp>
#include <vset/comparators/compare_member.hpp>

struct cmp_by_src:
  vset::compare_list<
    vset::compare_member< hit, uint32_t, &hit::src_id, std::less<uint32_t> >,
    vset::compare_member< hit, uint32_t, &hit::dst_id, std::less<uint32_t> >
  > {};
  
struct cmp_by_dst:
  vset::compare_list<
    vset::compare_member< hit, uint32_t, &hit::dst_id, std::less<uint32_t> >,
    vset::compare_member< hit, time_t,   &hit::ts,     std::greater<time_t> >,
    vset::compare_member< hit, uint32_t, &hit::src_id, std::less<uint32_t> >
  > {};

struct cmp_by_ts:
  vset::compare_list<
    vset::compare_member< hit, time_t,   &hit::ts,     std::greater<time_t> >,
    vset::compare_member< hit, uint32_t, &hit::src_id, std::less<uint32_t> >,
    vset::compare_member< hit, uint32_t, &hit::dst_id, std::less<uint32_t> >
  > {};
