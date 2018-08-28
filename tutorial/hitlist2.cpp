#include "hitlist.hpp"
#include "hitlist_t.hpp"
#include <hitlist/compare.hpp>
#include <vset/comparators/pointer_compare.hpp>
#include <set>
#include <iostream>
#include <limits>

class hitlist::impl
{
  struct storage
  {
    typedef hit value_type;
    typedef hit* pointer;
    typedef const hit* const_pointer;
    
    pointer allocate( size_t) { return new hit(); }
    void deallocate(pointer p, size_t){ delete p; }
  };
  
  typedef storage::value_type value_type;
  typedef storage::pointer pointer;
  typedef storage::const_pointer const_pointer;
  typedef pointer index_type;

  struct params
  {
    typedef storage storage_type;
    typedef storage::pointer pointer;
    typedef std::multiset<index_type, vset::pointer_compare<hit_src_cmp> > src_index;
    typedef std::multiset<index_type, vset::pointer_compare<hit_dst_cmp> > dst_index;
    typedef std::multiset<index_type, vset::pointer_compare<hit_ts_cmp>  >  ts_index;
    
    static index_type get_index(pointer p) {return p;}
    static pointer get_pointer(index_type h,  storage_type&) {return h;}
    /*static value_type& get_ref(pointer h) {return *h;}
    static const value_type& get_ref(const_pointer h) {return *h;}*/

  };
  
  typedef params::src_index by_src_t;
  typedef params::dst_index by_dst_t;
  typedef params::ts_index  by_ts_t;
 
  explicit impl(const impl&) = delete;
  impl& operator=(const impl&) = delete;

public:
  
  impl()
    : _p1(new value_type()), _p2(new value_type())
    , _hitlist(_storage, _by_src, _by_dst, _by_ts, _p1, _p2 )
  { }
  
 
  ~impl()
  {
    delete _p1; 
    delete _p2; 
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
    return (32 + sizeof(hit*)) * _hitlist.size() * 3;
  }
  
#include "hitlist_methods.inl"
  
private:
  storage _storage;
  by_src_t _by_src;
  by_dst_t _by_dst;
  by_ts_t  _by_ts;
  index_type _p1;
  index_type _p2;
  hitlist_t<params> _hitlist;
};

#include "hitlist_impl.inl"
