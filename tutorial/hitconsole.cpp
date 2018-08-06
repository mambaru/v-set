#include "hitlist.hpp"
#include "hitlist/compare.hpp"
#include <utility>
#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <deque>
#include <chrono>

hitlist storage;
std::deque<hit> hit_deque;
std::set<uint32_t> ids;

typedef std::list<std::string> arg_list;

namespace{
  
bool enabled_trace = false;
const std::string generate_file = "./generate.txt";

template<typename T>
T extract_param( arg_list& args )
{
  if ( args.empty() )
    return T();
  T value = static_cast<T>( std::atol(args.front().c_str() ) );
  args.pop_front();
  return static_cast<T>(value);
}

std::chrono::high_resolution_clock::time_point start()
{
  return std::chrono::high_resolution_clock::now();  
}

time_t finish(const std::chrono::high_resolution_clock::time_point& beg)
{
  auto end = std::chrono::high_resolution_clock::now();  
  return std::chrono::duration_cast<std::chrono::microseconds>(end-beg).count();
}

size_t to_rate(time_t span, size_t count)
{
  return count * 1000000 / static_cast<size_t>(span);
}

void help()
{
  
}

void generate( arg_list& arg)
{
  std::cout << "generate" << std::endl;
  int hits_total = extract_param<int>(arg);
  int users = extract_param<int>(arg);
  int interval = extract_param<int>(arg);
  std::set<hit, cmp_by_src> hits;
  
  while( hits.size() <  static_cast<size_t>(hits_total) )
  {
    // может смотреть сам себя
    hits.insert( hit::make( 
      static_cast<uint32_t>(rand()%users), 
      static_cast<uint32_t>(rand()%users), 
      static_cast<time_t>(rand()%interval)
    ));
  }
  
  std::ofstream of(generate_file);
  of << hits_total << " " << users << " " << interval << std::endl;
  for (auto h : hits)
  {
    of << h.src_id << " " << h.dst_id << " " << h.ts << std::endl;
  }
}

void load(arg_list&)
{
  hit_deque.clear();
  int hits_total = 0;
  int users = 0;
  int interval =0;
  std::ifstream ifs(generate_file);  
  ifs >> hits_total >> users >> interval ;
  if ( enabled_trace )
    std::cout << hits_total << " " << users << " " << interval << std::endl;
  
  std::cout << "load from generate.txt" << std::endl;
  std::cout << "\ttotal = " << hits_total << std::endl;
  std::cout << "\tusers = " << users << std::endl;
  std::cout << "\tinterval = " << interval << std::endl;
  
  auto beg = start();
  while(ifs)
  {
    hit h;
    ifs >> h.src_id >> h.dst_id >> h.ts ;
    if ( ifs )
    {
      hit_deque.push_back(h);
      ids.insert(h.src_id);
      ids.insert(h.dst_id);
    }
    
    if ( enabled_trace )
      std::cout << h.src_id << " " << h.dst_id << " " << h.ts << std::endl;
  }
  auto span = finish(beg);
  auto rate = to_rate(span, hit_deque.size());
  std::cout << "\tloaded! size=" << hit_deque.size() << " time=" << span << "mks rate=" << rate << std::endl;

}

void initialize( arg_list& )
{
  if ( hit_deque.empty() )
  {
    arg_list al;
    load(al);
  }
  
  std::cout << "initialize..." << std::endl;
  auto beg = start();
  for (const hit& h : hit_deque)
    storage.set_hit( h.src_id, h.dst_id, h.ts);
  auto span = finish(beg);
  auto rate = to_rate(span, hit_deque.size());
  std::cout << "\tinitialize! time=" << span << "mks rate=" << rate << std::endl;
}

void remove( arg_list& )
{
  if ( hit_deque.empty() )
  {
    arg_list al;
    initialize(al);
  }
  
  std::cout << "delete all users one by one" << std::endl;
  auto beg = start();
  size_t count = 0;
  size_t total = 0;
  for ( const uint32_t& id: ids )
    if ( size_t dels = storage.delete_user(id) )
    {
      count++;
      total+=dels;
    }
  auto span = finish(beg);
  auto rate = to_rate(span, ids.size());
  std::cout << "\ndeleted! calls=" << ids.size() << " deleted users=" << count << " time=" << span << "mks rate=" << rate << " remove hits=" << total << std::endl;
}

void size( arg_list& )
{
  std::cout << "size=" <<  storage.size() << std::endl;
}

void capacity( arg_list& )
{
  std::cout << "capacity=" <<  storage.capacity() << std::endl;
}

}
int main(int argc, char* argv[])
{
  arg_list argl(argv + 1, argv + argc);
  while ( !argl.empty() )
  {
    std::string name = argl.front();
    argl.pop_front();
    
    if ( name == "help" )
    {
      help();
    }
    else if ( name == "trace" )
    {
      enabled_trace = true;
    }
    else if (name == "generate")
    {
      generate(argl);
    }
    else if (name == "load")
    {
      load(argl);
    }
    else if (name == "initialize")
    {
      initialize(argl);
    }
    else if (name == "size")
    {
      size(argl);
    }
    else if (name == "capacity")
    {
      capacity(argl);
    }
    else if (name == "delete")
    {
      remove(argl);
    }
    
  }
  return 1;
}
