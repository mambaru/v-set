#include <vset/multiset.hpp>
#include <vset/allocators/mmap_allocator.hpp>
#include <vset/comparators/compare.hpp>
#include <vset/comparators/compare_list.hpp>
#include <vset/comparators/compare_member.hpp>
#include <fas/xtime.hpp>
#include <fas/typemanip.hpp>
#include <fas/functional.hpp>
#include <functional>
#include <iostream>
#include <set>
#include <list>

#include "config.hpp"
#include "data.hpp"

#define RANGE 3
#ifdef NDEBUG
  #define MAX_TEST 10
  #define MAX_DATA 100000/*000*/
#else
  #define MAX_TEST 1
  #define MAX_DATA 10000
#endif


struct cmp1
{
  bool operator()(const data& l, const data& r) const
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
  bool operator()(const data& l, const data& r) const
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
  bool operator()(const data& l, const data& r) const
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

//typedef vset::compare_list2<compare_list> cmp2t;
//typedef vset::compare_list<compare_list> cmp3t;

/*
typedef vset::compare_list2<  
  vset::compare_member<data, int, &data::data1, std::less<int> >,
  vset::compare_member<data, int, &data::data2, std::less<int> >,
  vset::compare_member<data, int, &data::data3, std::greater<int> >,
  vset::compare_member<data, int, &data::data4, std::less<int> >,
  vset::compare_member<data, int, &data::data5, std::less<int> >,
  vset::compare_member<data, int, &data::data6, std::less<int> >
> cmp2t;

typedef vset::compare_list<  
  vset::compare_member<data, int, &data::data1, std::less<int> >,
  vset::compare_member<data, int, &data::data2, std::less<int> >,
  vset::compare_member<data, int, &data::data3, std::greater<int> >,
  vset::compare_member<data, int, &data::data4, std::less<int> >,
  vset::compare_member<data, int, &data::data5, std::less<int> >,
  vset::compare_member<data, int, &data::data6, std::less<int> >
> cmp3t;
*/


//__attribute__ ((noinline))
 inline bool cmp3f(const data& l, const data& r);

 //__attribute__ ((noinline))
 inline bool cmp3f(const data& l, const data& r) 
{
  return cmp3()(l, r);
}

extern "C"
{
  bool cmp3fe(const data& l, const data& r);
}

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

struct ibench
{
  virtual ~ibench() {}
  virtual void run(bool show) = 0;
  virtual void run_lambda() {};
  virtual void show() = 0;
  virtual bool equal(ibench*) = 0;
};

struct ibench_data: ibench
{
  virtual ~ibench_data() {}
  virtual std::vector<data> get_data() = 0;
  virtual void show_result() = 0;
};

template<typename V, typename C, typename I>
class bench_base: public I
{
public:
  bench_base(std::vector<V>& orig, const C& cmp, const std::string& desc)
    : _orig(orig), _compare(cmp), _desc(desc)
  {}
  
  virtual void run(bool show)
  {
    _res.assign(_orig.begin(), _orig.end());
    fas::nanospan start = fas::nanotime();
    std::sort(_res.begin(), _res.end(), _compare);
    fas::nanospan finish = fas::nanotime();
    fas::nanospan span = finish - start;
    if (show) show_(span);
    _times.insert(span);
  }
  
  virtual void show()
  {
    show_( *(_times.begin()) );
  }
  
private:
  void show_(const fas::nanospan& span) const
  {
    std::cout << _desc << ": " << span << "s " /*<< typeid(C).name()*/ <<  std::endl;
    std::cout.flush();
  }
protected:
  std::vector<V>& _orig;
  C _compare;
  std::string _desc;
  std::set<fas::nanospan> _times;
  std::vector<V> _res;
};

template<typename V, typename C>
class bench_t: public bench_base<V, C, ibench>
{
public:
  bench_t(std::vector<V>& orig, const C& cmp, const std::string& desc)
    : bench_base<V, C, ibench>(orig, cmp, desc)
  {}
  
  virtual bool equal(ibench*) 
  {
    return true;
  }
};


template<typename C>
class bench_data: public bench_base<data, C, ibench_data>
{
  typedef bench_base<data, C, ibench_data> super;
public:
  bench_data(std::vector<data>& orig, const C& cmp, const std::string& desc)
    : bench_base<data, C, ibench_data>(orig, cmp, desc)
  {}
  
  virtual void show_result()
  {
    for (auto i: super::_res) 
      std::cout << i.data1 << " " << i.data2 << " "<< i.data3 << " "<< i.data4 << " "<< i.data5 << " "<< i.data6 << " " << std::endl;
  }
  
  virtual bool equal(ibench* ab) 
  {
    if ( auto b = dynamic_cast<ibench_data*>(ab) )
    {
      return super::_res == b->get_data();
    }
    return true;
  }
  
  virtual std::vector<data> get_data() 
  {
    return super::_res;
  }
};

class bench_lambda: public bench_data<cmp3>
{
  typedef bench_data<cmp3> super;
public:
  bench_lambda(std::vector<data>& orig,  const std::string& desc)
    : super(orig, cmp3(), desc)
  {}
  
  virtual void run(bool) {}
  virtual void run_lambda() 
  {
    _res.assign(_orig.begin(), _orig.end());
    fas::nanospan start = fas::nanotime();
    std::sort(_res.begin(), _res.end(), [](const data& a, const data& b)
    {
      return cmp3()(a, b);
    });
    fas::nanospan finish = fas::nanotime();
    fas::nanospan span = finish - start;
    _times.insert(span);
  };

};


int main()
{
  std::srand(42);
  int rnd = std::rand();
  std::cout << rnd << std::endl;
  if (rnd!=71876166)
    abort();
  std::vector<data> d;
  d.resize(MAX_DATA);
  std::generate(d.begin(), d.end(), f_generate());
  
  typedef std::tuple<int, int, int, int, int, int> data_tuple;
  std::vector<data_tuple> dt;
  dt.reserve(d.size());
  for ( const data& v: d )
    dt.push_back(std::make_tuple(v.data1, v.data2, v.data3, v.data4, v.data5, v.data6) );
  
  std::vector<ibench*> benches;
  benches.push_back( new bench_t<data_tuple, std::less<data_tuple> >(dt, std::less<data_tuple>(), "tuple" ) );
  
  
  benches.push_back( new bench_data< cmp1 >(d, cmp1(), "functor(1) if") );
  benches.push_back( new bench_data< cmp2 >(d, cmp2(), "functor(2) ||") );
  benches.push_back( new bench_data< cmp3 >(d, cmp3(), "functor(3) ?:") );
  
  typedef bool (*cmp_fun1)(const data&, const data&);
  benches.push_back( new bench_data<cmp_fun1>(d, &cmp3f, "inline function(3)" ) );
  benches.push_back( new bench_data<cmp_fun1>(d, &cmp3fe, "extern function(3)" ) );
  
  typedef std::function<bool(const data&, const data&)> cmp_fun_t;
  benches.push_back( new bench_data<cmp_fun_t>(d, &cmp3f, "std::function(function(3))") );
  
  cmp_fun_t cmpf3l = [](const data& a, const data& b){
    return cmp3t()(a, b);
  };
  benches.push_back( new bench_data< cmp_fun_t >(d, cmpf3l, "std::function(lambda(3))") );

  cmp_fun_t cmpf3o = cmp3();
  benches.push_back( new bench_data< cmp_fun_t >(d, cmpf3o, "std::function(object(3))") );

    benches.push_back( new bench_lambda(d, "lambda(3) inline") );
  auto cmp3l = [](const data& a, const data& b){
    return cmp3()(a, b);
  };
  benches.push_back( new bench_data< decltype(cmp3l) >(d, cmp3l, "lambda(3)") );




  benches.push_back( new bench_data< cmp3t >(d, cmp3t(), "Шаблонный Функтор(?:)") );
  benches.push_back( new bench_data< cmp2t >(d, cmp2t(), "Шаблонный Функтор(&&)") );
  

  for (int i = 0; i < MAX_TEST; ++i)
  {
    //std::cout << "-----------" << i  << "(" << MAX_TEST << ")------------" << std::endl;
    for (std::vector<ibench*>::iterator itr = benches.begin(); itr!=benches.end(); ++itr)
    {
      (*itr)->run_lambda();
      (*itr)->run(false);
    }
    //benches.back()->show_result();
  }

  
  std::cout << "###########################" << std::endl;
  std::cout << "###########################" << std::endl;
  for (auto b1:  benches)
  {
      b1->show();
      for (auto b2:  benches)
        if (!b1->equal(b2))
          abort();
  }
  return 0;
}
