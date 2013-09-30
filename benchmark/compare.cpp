#include <vset/multiset.hpp>
#include <vset/allocators/mmap_allocator.hpp>
#include <vset/comparators/compare.hpp>
#include <vset/comparators/compare_list.hpp>
#include <fas/xtime.hpp>
#include <fas/typemanip.hpp>
#include <iostream>

#include "config.hpp"
//using namespace vset;

struct data
{
  int data1;
  int data2;
  int data3;
};


bool operator == (data l, data r)
{
  return l.data1==r.data1 && l.data2==r.data2 && l.data3==r.data3;
}


typedef fas::type_list_n<
  vset::compare< fas::member<data, int, &data::data1>, std::less<int> >,
  vset::compare< fas::member<data, int, &data::data2>, std::greater<int> >,
  vset::compare< fas::member<data, int, &data::data3>, std::less<int> >
>::type compare_list;

struct cmp_data1: vset::compare_list<compare_list> {};
struct cmp_data2: vset::compare_list2<compare_list> {};


struct cmp_data3
{
  bool operator()(const data& l, const data& r) const
  {
    //std::cout << l.data1 << std::endl;
    return l.data1 < r.data1 ||
    ( ! ( r.data1 < l.data1 ) && l.data2 > r.data2 ) ||
    ( ! ( r.data1 < l.data1 ) && ! ( r.data2 > l.data2 ) && l.data3 < r.data3 );
  }
};


#define MAX_DATA 10000000
int main()
{
  std::vector<data> d1, d2, d3;
  d1.resize(MAX_DATA);
  std::generate(d1.begin(), d1.end(), [](){return data{std::rand()%100, std::rand()%100, std::rand()%100};});
  d2 = d1;
  d3 = d1;
  
  fas::nanospan start = fas::nanotime();
  std::sort(d1.begin(), d1.end(), cmp_data1());
  fas::nanospan finish = fas::nanotime();
  std::cout << finish - start << std::endl;
  
  start = fas::nanotime();
  std::sort(d2.begin(), d2.end(), cmp_data2());
  finish = fas::nanotime();
  std::cout << finish - start << std::endl;

  start = fas::nanotime();
  std::sort(d3.begin(), d3.end(), cmp_data3());
  finish = fas::nanotime();
  std::cout << finish - start << std::endl;

  if ( !(d1==d2) )
    abort();

  if ( !(d1==d3) )
    abort();

  return 0;
}