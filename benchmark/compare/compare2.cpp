
#include "data.hpp"
#include "compare.hpp"

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


#define RANGE 3
#ifdef NDEBUG
  #define MAX_TEST 10
  #define MAX_DATA 100000/*000*/
#else
  #define MAX_TEST 1
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

void show(const std::string& text, fas::nanospan span);
void show(const std::string& text, fas::nanospan span)
{
  std::cout << text << " " << span << std::endl;
}

struct ibench
{
  virtual ~ibench() {}
  virtual void run(bool show_flag) = 0;
  virtual void run_lambda() {};
  virtual void show() = 0;
  virtual bool equal(ibench*) = 0;
};

struct ibench_data: ibench
{
  virtual ~ibench_data() {}
  virtual std::vector<data> get_data() = 0;
  //virtual void show_result() = 0;
};

template<typename V, typename C, typename I>
class bench_base: public I
{
public:
  bench_base(std::vector<V>& orig, const C& cmp, const std::string& desc)
    : _orig(orig), _compare(cmp), _desc(desc)
  {}
  
  virtual void run(bool show_flag) override
  {
    _res.assign(_orig.begin(), _orig.end());
    fas::nanospan start = fas::nanotime();
    std::sort(_res.begin(), _res.end(), _compare);
    fas::nanospan finish = fas::nanotime();
    fas::nanospan span = finish - start;
    if (show_flag) show_(span);
    _times.insert(span);
  }
  
  virtual void show() override
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
  
  virtual bool equal(ibench*) override
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
  
  /*
  virtual void show_result()
  {
    for (auto i: super::_res) 
      std::cout << i.data1 << " " << i.data2 << " "<< i.data3 << " "<< i.data4 << " "<< i.data5 << " "<< i.data6 << " " << std::endl;
  }*/
    
  virtual bool equal(ibench* ab) override
  {
    if ( auto b = dynamic_cast<ibench_data*>(ab) )
    {
      return super::_res == b->get_data();
    }
    return true;
  }
  
  virtual std::vector<data> get_data() override
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
  
  virtual void run(bool) override {}
  
  virtual void run_lambda() override
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
