#include "hitlist.hpp"
#include "hitlist_t.hpp"
#include <hitlist/compare.hpp>
#include <set>
#include <iostream>
#include <limits>

class hitlist::impl
{
  struct storage
  {
    typedef hit value_type;
    struct pointer
    {
      explicit pointer(hit& h): _h(h) {}
      hit& operator*() { return _h;}
      const hit& operator*() const { return _h;}
    private:
      hit& _h;
    };
    
    pointer allocate( size_t) 
    {
      static hit fake;
      return pointer(fake); 
    }
    void deallocate(pointer, size_t){}
  };
  
  typedef storage::value_type value_type;
  typedef storage::pointer pointer;
  //typedef storage::const_pointer const_pointer;
  typedef value_type index_type;

  
  struct params
  {
    
    typedef storage storage_type;
    typedef storage::pointer pointer;
    typedef std::multiset<index_type, hit_src_cmp> src_index;
    typedef std::multiset<index_type, hit_dst_cmp> dst_index;
    typedef std::multiset<index_type, hit_ts_cmp >  ts_index;
    
    static index_type get_index(pointer h) {return *h;}
    static pointer get_pointer(index_type& h,  storage_type&) {return pointer(h);}
    /*static value_type& get_ref(pointer h) {return *h;}
    static const value_type& get_ref(const_pointer h) {return *h;}*/
  };
  
  typedef params::src_index by_src_t;
  typedef params::dst_index by_dst_t;
  typedef params::ts_index  by_ts_t;
  
public:
  
  impl()
    : _p1(_h1), _p2(_h1)
    , _hitlist(_storage, _by_src, _by_dst, _by_ts, _p1, _p2 )
  { }
  
  ~impl()
  {
  }
  
  bool open(size_t, size_t)
  {
    return false;
  }
  
  std::string desc() const
  {
    return "std::multiset<hit>";
  }
  
  size_t capacity() const
  {
    return (32 + sizeof(hit)) * _by_src.size() * 3;
  }
  
#include "hitlist_methods.inl"
  
private:
  storage _storage;
  by_src_t _by_src;
  by_dst_t _by_dst;
  by_ts_t  _by_ts;
  hit _h1, _h2;
  pointer _p1, _p2;

  hitlist_t<params> _hitlist;
};

#include "hitlist_impl.inl"
