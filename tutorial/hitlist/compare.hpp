#pragma once

#include <hitlist/hit.hpp>
#include <vset/comparators/compare_list.hpp>
#include <vset/comparators/compare_member.hpp>

template<typename T>
struct src_compare_t:
  vset::compare_list<
    vset::compare_member< T, uint32_t, &T::src_id, std::less<uint32_t> >,
    vset::compare_member< T, uint32_t, &T::dst_id, std::less<uint32_t> >
  > {};
  
template<typename T>
struct dst_compare_t:
  vset::compare_list<
    vset::compare_member< T, uint32_t, &T::dst_id, std::less<uint32_t> >,
    vset::compare_member< T, time_t,   &T::ts,     std::greater<time_t> >,
    vset::compare_member< T, uint32_t, &T::src_id, std::less<uint32_t> >
  > {};

template<typename T>
struct ts_compare_t:
  vset::compare_list<
    vset::compare_member< T, time_t,   &T::ts,     std::greater<time_t> >,
    vset::compare_member< T, uint32_t, &T::src_id, std::less<uint32_t> >,
    vset::compare_member< T, uint32_t, &T::dst_id, std::less<uint32_t> >
  > {};

struct hit_src_cmp: src_compare_t<hit> {};
struct hit_dst_cmp: dst_compare_t<hit> {};
struct hit_ts_cmp : ts_compare_t<hit> {};

