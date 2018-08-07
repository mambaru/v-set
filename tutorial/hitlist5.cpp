#include "hitlist.hpp"
#include "hitlist_t.hpp"
#include <hitlist/compare.hpp>
#include <vset/comparators/pointer_compare.hpp>
#include <vset/memory/fsb_mmap.hpp>

#include <set>
#include <iostream>
#include <limits>
#include <memory>

class hitlist::impl
{
  const std::string name="hitlist5";
  // Охринеть, это работает?
  typedef vset::memory::fsb_mmap<hit> storage;
  typedef storage::value_type value_type;
  typedef storage::pointer pointer;
  typedef value_type* index_type;
  /*typedef const value_type* const_pointer;*/
  
  struct params
  {
    typedef storage storage_type;
    /*typedef storage::pointer pointer;*/
    typedef std::multiset<index_type, vset::pointer_compare<hit_src_cmp> > src_index;
    typedef std::multiset<index_type, vset::pointer_compare<hit_dst_cmp> > dst_index;
    typedef std::multiset<index_type, vset::pointer_compare<hit_ts_cmp>  >  ts_index;
    
    static index_type get_index(pointer p) {return &(*p);}
    static pointer get_pointer(index_type i,  storage_type& stg) 
    {
      pointer p=stg.end();
      p.set_address(i);
      return p;
    }
    
  };
  
  typedef params::src_index by_src_t;
  typedef params::dst_index by_dst_t;
  typedef params::ts_index  by_ts_t;
  
public:
  
  impl()
    : /*_p1(new value_type())
    , _p2(new value_type())
    ,*/ _hitlist(_storage, _by_src, _by_dst, _by_ts, _p1, _p2 )
  { }

  ~impl()
  {
    /*delete _p1;
    delete _p2;*/
  }
  
  bool open(size_t reserve1, size_t)
  {
    std::string filename = name+".stg";
    _storage.buffer().open( filename.c_str() );
    _storage.buffer().reserve(reserve1);
    auto beg = _storage.begin();
    auto end = _storage.end();
    for (;beg!=end; ++beg)
    {
      _by_src.insert(beg);
      _by_dst.insert(beg);
      _by_ts.insert(beg);
    }
    if ( _storage.empty() )
    {
      _p1 = _storage.allocate(1);
      _p2 = _storage.allocate(1);
    }
    else
    {
      _p1 = _storage.begin();
      _p2 = _storage.begin() + 1;
    }
    return true;
  }
  
  std::string desc() const
  {
    return "std::multiset<hit>";
  }
  
  size_t capacity() const
  {
    return (32 + sizeof(hit*)) * _hitlist.size() * 3;
  }
  
#include "hitlist_methods.inl"
  
private:
  storage _storage;
  by_src_t _by_src;
  by_dst_t _by_dst;
  by_ts_t  _by_ts;
  
  pointer _p1;
  pointer _p2;

  hitlist_t<params> _hitlist;
};

#include "hitlist_impl.inl"
