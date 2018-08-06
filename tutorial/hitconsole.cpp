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
std::set<uint32_t> src_ids;
std::set<uint32_t> dst_ids;

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
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end-beg).count();
}

size_t to_rate(time_t span, size_t count)
{
  if ( span == 0 )
    return static_cast<size_t>(~0);
  return count * 1000000000 / static_cast<size_t>(span);
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
  if ( !ifs )
  {
    ifs.close();
    arg_list al;
    al.push_back("1000000");
    al.push_back("10000");
    al.push_back("10000");
    generate(al);
    ifs.open(generate_file);
  }

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
      src_ids.insert(h.src_id);
      dst_ids.insert(h.dst_id);
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
  if ( src_ids.empty() )
  {
    arg_list al;
    initialize(al);
  }

  {
    std::cout << "delete src users one by one" << std::endl;
    auto beg = start();
    size_t count = 0;
    for ( const uint32_t& id: src_ids )
      if ( storage.delete_user(id) )
        count++;
    auto span = finish(beg);
    auto rate = to_rate(span, src_ids.size());
    std::cout << "\tdone for src! calls=" << src_ids.size() 
              << " deleted users=" << count << " time=" << span << "mks rate=" << rate << std::endl;
  }
  
  {
    std::cout << "delete dst users one by one" << std::endl;
    auto beg = start();
    size_t count = 0;
    for ( const uint32_t& id: dst_ids )
      if ( storage.delete_user(id) )
        count++;
    auto span = finish(beg);
    auto rate = to_rate(span, dst_ids.size());
    std::cout << "\tdone for dst! calls=" << dst_ids.size() 
              << " deleted users=" << count << " time=" << span << "mks rate=" << rate << std::endl;
  }
}

void get_hits( arg_list& args)
{
  if ( dst_ids.empty() )
  {
    arg_list al;
    initialize(al);
  }
  
  size_t limit = extract_param<size_t>(args);
  
  std::cout << "get_hits all users one by one" << std::endl;
  auto beg = start();
  size_t count = 0;
  std::vector<hit>  hits;
  hits.reserve(limit);
  for ( const uint32_t& id: dst_ids )
  {
    storage.get_hits(hits, id, 0ul, limit);
    if ( !hits.empty() )
      ++count;
    if (enabled_trace)
      std::cout << "id=" << id << " size=" <<  hits.size() << std::endl;
    hits.clear();
  }
      
  auto span = finish(beg);
  auto rate = to_rate(span, dst_ids.size());
  std::cout << "\tdone! calls=" << dst_ids.size() << " deleted users=" << count << " time=" << span << "mks rate=" << rate << std::endl;
}

void src_count( arg_list& args)
{
  if ( dst_ids.empty() )
  {
    arg_list al;
    initialize(al);
  }
  
  size_t count = 0ul;
  size_t calls = 0ul;
  auto id = extract_param<uint32_t>(args);
  if ( id == 0 )
    std::cout << "src_count all users one by one" << std::endl;
  else
    std::cout << "src_count for user = " << id << std::endl;
  auto beg = start();
  if (id!=0)
  {
    count = storage.src_count(id);
    calls=1;
  }
  else
  {
    for ( const uint32_t& idx: src_ids )
    {
      count+=storage.src_count(idx);
      ++calls;
      //std::cout << count << std::endl;
    }
  }
  auto span = finish(beg);
  auto rate = to_rate(span, calls);
  std::cout << "\tdone! calls=" << calls << " total counts=" << count << " time=" << span << "mks rate=" << rate << std::endl;

}

void dst_count( arg_list& )
{
  if ( dst_ids.empty() )
  {
    arg_list al;
    initialize(al);
  }
}

void outdated_count( arg_list& )
{
  if ( dst_ids.empty() )
  {
    arg_list al;
    initialize(al);
  }
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
    else if (name == "get_hits")
    {
      get_hits(argl);
    }
    else if (name == "src_count")
    {
      src_count(argl);
    }
    else if (name == "dst_count")
    {
      dst_count(argl);
    }
    else if (name == "outdated_count")
    {
      outdated_count(argl);
    }
     
 
    
  }
  return 1;
}
