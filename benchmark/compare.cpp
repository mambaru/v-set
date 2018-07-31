#include <vset/multiset.hpp>
#include <vset/allocators/mmap_allocator.hpp>
#include <vset/comparators/compare.hpp>
#include <vset/comparators/compare_list.hpp>
#include <fas/xtime.hpp>
#include <fas/typemanip.hpp>
#include <fas/functional.hpp>
#include <iostream>
#include <set>

#include "config.hpp"

struct data
{
  int data1;
  int data2;
  int data3;
  int data4;
  int data5;
  int data6;
};

inline bool operator == (const data& l, const data& r)
{
  return l.data1==r.data1 && l.data2==r.data2 && l.data3==r.data3
        && l.data4==r.data4 && l.data5==r.data5 && l.data6==r.data6;
}


typedef fas::type_list_n<
  vset::compare< fas::member<data, int, &data::data1>, std::less<int> >,
  vset::compare< fas::member<data, int, &data::data2>, std::greater<int> >,
  vset::compare< fas::member<data, int, &data::data3>, std::less<int> >,
  vset::compare< fas::member<data, int, &data::data4>, std::less<int> >,
  vset::compare< fas::member<data, int, &data::data5>, std::less<int> >,
  vset::compare< fas::member<data, int, &data::data6>, std::less<int> >
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
    ( ! ( r.data1 < l.data1 ) && !( r.data2 > l.data2 ) && l.data3 < r.data3 ) ||
    ( ! ( r.data1 < l.data1 ) && !( r.data2 > l.data2 ) && !( r.data3 < l.data3 ) && l.data4 < r.data4 ) ||
    ( ! ( r.data1 < l.data1 ) && !( r.data2 > l.data2 ) && !( r.data3 < l.data3 ) && !( r.data4 < l.data4 ) && l.data5 < r.data5 ) ||
    ( ! ( r.data1 < l.data1 ) && !( r.data2 > l.data2 ) && !( r.data3 < l.data3 ) && !( r.data4 < l.data4 ) && !( r.data5 < l.data5 ) && l.data6 < r.data6 )
    ;
  }
};

struct cmp_data4
{
  bool operator()(const data& l, const data& r) const
  {
    if ( l.data1 < r.data1 ) return true;
    if ( l.data1 > r.data1 ) return false;
    
    if ( l.data2 > r.data2 ) return true;
    if ( l.data2 < r.data2 ) return false;
    
    if ( l.data3 < r.data3 ) return true;
    if ( l.data3 > r.data3 ) return false;
    
    if ( l.data4 < r.data4 ) return true;
    if ( l.data4 > r.data4 ) return false;
    
    if ( l.data5 < r.data5 ) return true;
    if ( l.data5 > r.data5 ) return false;    
    
    if ( l.data6 < r.data6 ) return true;
    if ( l.data6 > r.data6 ) return false;
    
    return false;
  }
};

#define RANGE 10
#ifdef NDEBUG
  #define MAX_TEST 5
  #define MAX_DATA 10000000
#else
  #define MAX_TEST 1
  #define MAX_DATA 1000
#endif

struct f_generate
{
  data operator()() const
  {
    data d;
    d.data1 = std::rand()%RANGE;
    d.data2 = std::rand()%RANGE;
    d.data3 = std::rand()%RANGE;
    d.data4 = std::rand()%RANGE;
    d.data5 = std::rand()%RANGE;
    d.data6 = std::rand()%RANGE;
    return d;
  }
};

int main()
{
  std::srand(time(0));
  std::vector<data> d;
  d.resize(MAX_DATA);
  std::generate(d.begin(), d.end(), f_generate());
  
  typedef std::tuple<int, int, int, int, int, int> data_tuple;
  std::vector<data_tuple> dt;
  dt.reserve(d.size());
  for ( const data& v: d )
    dt.push_back(std::make_tuple(v.data1, v.data2, v.data3, v.data4, v.data5, v.data6) );
  
  
  std::set<fas::nanospan> time1;
  std::set<fas::nanospan> time2;
  std::set<fas::nanospan> time3;
  std::set<fas::nanospan> time4;
  std::set<fas::nanospan> time5;
  for (int i = 0; i < MAX_TEST; ++i)
  {
    std::cout << "---------------------------" << std::endl;
    std::vector<data> d1, d2, d3, d4;
    std::vector<data_tuple> dt1;
    d1 = d;
    d2 = d;
    d3 = d;
    d4 = d;
    dt1 = dt;
    
    fas::nanospan start = fas::nanotime();
    std::sort(d1.begin(), d1.end(), cmp_data1());
    fas::nanospan finish = fas::nanotime();
    std::cout << finish - start << std::endl;
    time1.insert(finish - start);

    start = fas::nanotime();
    std::sort(dt1.begin(), dt1.end());
    finish = fas::nanotime();
    std::cout << finish - start << std::endl;
    time5.insert(finish - start);

    start = fas::nanotime();
    std::sort(d2.begin(), d2.end(), cmp_data2());
    finish = fas::nanotime();
    std::cout << finish - start << std::endl;
    time2.insert(finish - start);

    start = fas::nanotime();
    std::sort(d3.begin(), d3.end(), cmp_data3());
    finish = fas::nanotime();
    std::cout << finish - start << std::endl;
    time3.insert(finish - start);

    start = fas::nanotime();
    std::sort(d4.begin(), d4.end(), cmp_data4());
    finish = fas::nanotime();
    std::cout << finish - start << std::endl;
    time4.insert(finish - start);

    if ( !(d1==d2) )
    {
      abort();
    }

    if ( !(d1==d3) )
    {
      abort();
    }
  }

  std::cout << "---------------------------" << std::endl;
  std::cout << "---------------------------" << std::endl;
  std::cout << "---------------------------" << std::endl;

  std::cout << *(time1.begin()) << std::endl;
  std::cout << *(time2.begin()) << std::endl;
  std::cout << *(time3.begin()) << std::endl;
  std::cout << *(time4.begin()) << std::endl;
  std::cout << *(time5.begin()) << std::endl;

  return 0;
}
