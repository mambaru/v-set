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
T extract_param( arg_list& args, T defval = T() )
{
  if ( args.empty() )
    return defval;
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
  if ( span == 0 )
    return static_cast<size_t>(~0);
  return count * 1000000 / static_cast<size_t>(span);
}


}

void help();
void generate( arg_list&);
void load(arg_list&);
void open(arg_list&);
void initialize( arg_list& );
void prepare();

void remove( arg_list& );
void get_hits( arg_list&);
void src_count( arg_list&);
void dst_count( arg_list&);
void outdated_count( arg_list& );
void size( arg_list& );
void capacity( arg_list& );

void help()
{
  std::cout << storage.desc() << std::endl;
  std::cout << "\tgenerate" << std::endl;
  std::cout << "\tload"<< std::endl;
  std::cout << "\topen"<< std::endl;
  std::cout << "\tinitialize"<< std::endl;
  std::cout << "\tsize"<< std::endl;
  std::cout << "\tcapacity"<< std::endl;
  std::cout << "\tdelete"<< std::endl;
  std::cout << "\tget_hits"<< std::endl;
  std::cout << "\tsrc_count"<< std::endl;
  std::cout << "\tdst_count"<< std::endl;
  std::cout << "\toutdated_count"<< std::endl;
}

void generate( arg_list& arg)
{
  std::cout << "generate" << std::endl;
  int hits_total = extract_param<int>(arg, 1000000);
  int users = extract_param<int>(arg, 10000);
  int interval = extract_param<int>(arg, 10000);
  std::set<hit, hit_src_cmp> hits;
  
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
    return;

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

void open(arg_list&)
{
  auto beg = start();
  bool opened = storage.open(20000000, 20000000);
  auto span = finish(beg);
  auto rate = to_rate(span, 1);
  if ( opened )
  {
    std::cout << "\topened! size=" << storage.size() << " time=" << span << "mks rate=" << rate << std::endl;
    std::cout << "\tinitialize..." << std::endl;
    storage.for_each([&](hit h){
      hit_deque.push_back(h);
      src_ids.insert(h.src_id);
      dst_ids.insert(h.dst_id);
    });
    std::cout << "\tinitialize... done. size=" << hit_deque.size() << std::endl;
  }
}

void initialize( arg_list& )
{
  std::cout << "initialize..." << std::endl;
  auto beg = start();
  for (const hit& h : hit_deque)
    storage.set_hit( h.src_id, h.dst_id, h.ts);
  auto span = finish(beg);
  auto rate = to_rate(span, hit_deque.size());
  std::cout << "\tinitialize! time=" << span << "mks rate=" << rate << std::endl;
}

void prepare()
{
  arg_list al;
  open(al);
  if ( hit_deque.size()!=0 )
    return;
  
  load(al);
  if ( hit_deque.size()==0 )
  {
    generate(al);
    load(al);
  }
  initialize(al);
}


void remove( arg_list& )
{
  prepare();
  {
    std::cout << "delete src users one by one" << std::endl;
    auto beg = start();
    size_t count = 0;
    for ( const uint32_t& id: src_ids )
      count += storage.delete_user(id);
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
      count += storage.delete_user(id);
    auto span = finish(beg);
    auto rate = to_rate(span, dst_ids.size());
    std::cout << "\tdone for dst! calls=" << dst_ids.size() 
              << " deleted users=" << count << " time=" << span << "mks rate=" << rate << std::endl;
  }
}

void get_hits( arg_list& args)
{
  prepare();
  
  size_t limit = extract_param<size_t>(args);
  if ( limit==0 ) limit = 10;
  
  std::cout << "get_hits all users one by one" << std::endl;
  auto beg = start();
  size_t count = 0;
  size_t total = 0;

  std::vector<hit>  hits;
  hits.reserve(limit);
  for ( const uint32_t& id: dst_ids )
  {
    hits.clear();
    storage.get_hits(hits, id, 0ul, limit);
    if ( !hits.empty() )
      ++count;
    total += hits.size();
    if (enabled_trace)
      std::cout << "id=" << id << " size=" <<  hits.size() << std::endl;
    hits.clear();
  }
      
  auto span = finish(beg);
  auto rate = to_rate(span, dst_ids.size());
  std::cout << "\tdone! calls=" << dst_ids.size() << " get_hits=" << count << " total hits=" << total << " time=" << span << "mks rate=" << rate << std::endl;
}

void src_count( arg_list& args)
{
  prepare();
  
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
    }
  }
  auto span = finish(beg);
  auto rate = to_rate(span, calls);
  std::cout << "\tdone! calls=" << calls << " total counts=" << count << " time=" << span << "mks rate=" << rate << std::endl;
}

void dst_count( arg_list& args)
{
  prepare();
  
  size_t count = 0ul;
  size_t calls = 0ul;
  auto id = extract_param<uint32_t>(args);
  if ( id == 0 )
    std::cout << "dst_count all users one by one" << std::endl;
  else
    std::cout << "dst_count for user = " << id << std::endl;
  auto beg = start();
  if (id!=0)
  {
    count = storage.dst_count(id);
    calls=1;
  }
  else
  {
    for ( const uint32_t& idx: dst_ids )
    {
      if ( enabled_trace )
        std::cout << "id=" << idx << std::endl;

      count+=storage.dst_count(idx);
      ++calls;
    }
  }
  auto span = finish(beg);
  auto rate = to_rate(span, calls);
  std::cout << "\tdone! calls=" << calls << " total counts=" << count << " time=" << span << "mks rate=" << rate << std::endl;
}

void outdated_count( arg_list& args)
{
  prepare();
  auto ts = extract_param<time_t>(args); 
  
  size_t calls = 0ul;
  std::cout << "outdated_count for time = " << ts << std::endl;
  auto beg = start();
  size_t count = storage.outdated_count(ts);
  ++calls;
  auto span = finish(beg);
  auto rate = to_rate(span, calls);
  std::cout << "\tdone! calls=" << calls << " total counts=" << count << " time=" << span << "mks rate=" << rate << std::endl;
}



void size( arg_list& )
{
  prepare();
  std::cout << "size=" <<  storage.size() << std::endl;
}

void capacity( arg_list& )
{
  prepare();
  std::cout << "capacity=" <<  storage.capacity() << std::endl;
}


int main(int argc, char* argv[])
{
  arg_list argl(argv + 1, argv + argc);
  if (argl.empty())
    argl.push_back("help");
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
    else if (name == "open")
    {
      open(argl);
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
