#pragma once
#include <fas/type_list.hpp>
#include <vset/comparators/compare.hpp>
#include <vset/comparators/compare_list.hpp>
#include <vset/comparators/compare_member.hpp>

#include "data.hpp"

struct cmp1
{
  bool operator()(const data& l, const data& r) const noexcept
  {
    if ( l.data1 < r.data1 ) return true;
    if ( l.data1 > r.data1 ) return false;
    
    if ( l.data2 < r.data2 ) return true;
    if ( l.data2 > r.data2 ) return false;
    
    if ( l.data3 > r.data3 ) return true;
    if ( l.data3 < r.data3 ) return false;
    
    if ( l.data4 < r.data4 ) return true;
    if ( l.data4 > r.data4 ) return false;
    
    if ( l.data5 < r.data5 ) return true;
    if ( l.data5 > r.data5 ) return false;    
    
    if ( l.data6 < r.data6 ) return true;
    return false;
  }
};

struct cmp2
{
  bool operator()(const data& l, const data& r) const noexcept
  {
    return l.data1 < r.data1 
      || ( ! ( r.data1 < l.data1 ) && l.data2 < r.data2 ) 
      || ( ! ( r.data1 < l.data1 ) && !( r.data2 < l.data2 ) && l.data3 > r.data3 ) 
      || ( ! ( r.data1 < l.data1 ) && !( r.data2 < l.data2 ) && !( r.data3 > l.data3 ) && l.data4 < r.data4 ) 
      || ( ! ( r.data1 < l.data1 ) && !( r.data2 < l.data2 ) && !( r.data3 > l.data3 ) && !( r.data4 < l.data4 ) && l.data5 < r.data5 ) 
      || ( ! ( r.data1 < l.data1 ) && !( r.data2 < l.data2 ) && !( r.data3 > l.data3 ) 
               && !( r.data4 < l.data4 ) && !( r.data5 < l.data5 ) && l.data6 < r.data6 );
  }
};



struct cmp3
{
  bool operator()(const data& l, const data& r) const noexcept
  {
    return   l.data1 < r.data1 ? true : l.data1 > r.data1 ? false
           : l.data2 < r.data2 ? true : l.data2 > r.data2 ? false 
           : l.data3 > r.data3 ? true : l.data3 < r.data3 ? false 
           : l.data4 < r.data4 ? true : l.data4 > r.data4 ? false 
           : l.data5 < r.data5 ? true : l.data5 > r.data5 ? false 
           : l.data6 < r.data6 ? true : false;
  }
};


typedef fas::type_list_n<
  vset::compare_member<data, int, &data::data1, std::less<int> >,
  vset::compare_member<data, int, &data::data2, std::less<int> >,
  vset::compare_member<data, int, &data::data3, std::greater<int> >,
  vset::compare_member<data, int, &data::data4, std::less<int> >,
  vset::compare_member<data, int, &data::data5, std::less<int> >,
  vset::compare_member<data, int, &data::data6, std::less<int> >
>::type compare_list;

struct cmp2t: vset::compare_list2<compare_list> {};
struct cmp3t: vset::compare_list<compare_list> {};

inline bool cmp3f(const data& l, const data& r);
inline bool cmp3f(const data& l, const data& r) 
{
  return cmp3()(l, r);
}

extern "C"
{
  bool cmp3fe(const data& l, const data& r);
}

