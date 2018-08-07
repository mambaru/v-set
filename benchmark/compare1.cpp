#include <vset/multiset.hpp>
#include <vset/allocators/mmap_allocator.hpp>
#include <vset/comparators/compare.hpp>
#include <vset/comparators/compare_list.hpp>
#include <fas/xtime.hpp>
#include <fas/typemanip.hpp>
#include <fas/functional.hpp>
#include <functional>
#include <iostream>
#include <set>

#include "config.hpp"
#include "data.hpp"


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
    return l.data1 < r.data1 
      || ( ! ( r.data1 < l.data1 ) && l.data2 > r.data2 ) 
      || ( ! ( r.data1 < l.data1 ) && !( r.data2 > l.data2 ) && l.data3 < r.data3 ) 
      || ( ! ( r.data1 < l.data1 ) && !( r.data2 > l.data2 ) && !( r.data3 < l.data3 ) && l.data4 < r.data4 ) 
      || ( ! ( r.data1 < l.data1 ) && !( r.data2 > l.data2 ) && !( r.data3 < l.data3 ) && !( r.data4 < l.data4 ) && l.data5 < r.data5 ) 
      || ( ! ( r.data1 < l.data1 ) && !( r.data2 > l.data2 ) && !( r.data3 < l.data3 ) 
               && !( r.data4 < l.data4 ) && !( r.data5 < l.data5 ) && l.data6 < r.data6 )
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

inline bool cmp_data5(const data& l, const data& r)
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

#define RANGE 100
#ifdef NDEBUG
  #define MAX_TEST 10
  #define MAX_DATA 10000000
#else
  #define MAX_TEST 5
  #define MAX_DATA 10000
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

void show(const std::string& text, fas::nanospan span)
{
  std::cout << text << " " << span << std::endl;
}

const char* desc[]={
  "template 'multi if' compare ",
  "template 'one if' compare   ",
  "one 'if' compare            ",
  "multi 'if' compare          ",
  "multi 'if' compare fun      ",
  "tuple default compare       "
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
  std::set<fas::nanospan> time6;
  for (int i = 0; i < MAX_TEST; ++i)
  {
    std::cout << "-----------" << i  << "(" << MAX_TEST << ")------------" << std::endl;
    std::vector<data> d1, d2, d3, d4, d5;
    std::vector<data_tuple> dt1;
    d1 = d;
    d2 = d;
    d3 = d;
    d4 = d;
    d5 = d;
    dt1 = dt;
    

    //std::function<bool (const data&, const data&)> tmp1 = cmp_data1();
    fas::nanospan start = fas::nanotime();
    std::sort(d1.begin(), d1.end(), cmp_data1());
    /*std::sort(d1.begin(), d1.end(), [](const data& a, const data& b){
      return cmp_data1()(a, b);
    });*/
    fas::nanospan finish = fas::nanotime();
    show(desc[0],  finish - start );
    time1.insert(finish - start);

    start = fas::nanotime();
    std::sort(d2.begin(), d2.end(), cmp_data2());
    finish = fas::nanotime();
    show(desc[1],  finish - start );
    time2.insert(finish - start);

    start = fas::nanotime();
    std::sort(d3.begin(), d3.end(), cmp_data3());
    finish = fas::nanotime();
    show(desc[2],  finish - start );
    time3.insert(finish - start);

    start = fas::nanotime();
    std::sort(d4.begin(), d4.end(), cmp_data4());
    finish = fas::nanotime();
    show(desc[3],  finish - start );
    time4.insert(finish - start);

    start = fas::nanotime();
    std::sort(d5.begin(), d5.end(), cmp_data5);
    finish = fas::nanotime();
    show(desc[4],  finish - start );
    time5.insert(finish - start);

    start = fas::nanotime();
    std::sort(dt1.begin(), dt1.end());
    finish = fas::nanotime();
    show(desc[5],  finish - start );
    time6.insert(finish - start);

    std::cout << "========================" << std::endl;
    if ( !(d1==d2) )  abort();
    if ( !(d1==d3) )  abort();
    if ( !(d1==d4) )  abort();
    if ( !(d1==d5) )  abort();
  }

  std::cout << "---------------------------" << std::endl;
  std::cout << "---------------------------" << std::endl;
  std::cout << "---------------------------" << std::endl;

  show(desc[0], *(time1.begin()) );
  show(desc[1], *(time2.begin()) );
  show(desc[2], *(time3.begin()) );
  show(desc[3], *(time4.begin()) );
  show(desc[4], *(time5.begin()) );
  show(desc[5], *(time6.begin()) );
  return 0;
}
