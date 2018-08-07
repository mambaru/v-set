#include "hitlist.hpp"
#include "hitlist_t.hpp"
#include <hitlist/compare.hpp>
#include <vset/comparators/pointer_compare.hpp>
#include <set>
#include <iostream>
#include <limits>
#include <memory>

class hitlist::impl
{
  struct storage
  {
    typedef hit value_type;
    typedef std::shared_ptr<hit> pointer;
    typedef std::shared_ptr<const hit> const_pointer;
    
    pointer allocate( size_t) { return std::make_shared<hit>(); }
    void deallocate(pointer, size_t){ }
  };

  typedef storage::value_type value_type;
  typedef storage::pointer pointer;
  typedef storage::const_pointer const_pointer;
  
  struct params
  {
    typedef storage storage_type;
    typedef storage::pointer pointer;
    typedef std::multiset<pointer, vset::pointer_compare<hit_src_cmp> > src_index;
    typedef std::multiset<pointer, vset::pointer_compare<hit_dst_cmp> > dst_index;
    typedef std::multiset<pointer, vset::pointer_compare<hit_ts_cmp>  >  ts_index;
    
    static pointer get_index(pointer p) {return p;}
    static pointer get_pointer(pointer h,  storage_type&) {return h;}
    /*static value_type& get_ref(pointer h) {return *h;}*/
  };
  
  typedef params::src_index by_src_t;
  typedef params::dst_index by_dst_t;
  typedef params::ts_index  by_ts_t;
  
public:
  
  impl()
    : _ph1(std::make_shared<hit>()), _ph2(std::make_shared<hit>())
    , _hitlist(_storage, _by_src, _by_dst, _by_ts, _ph1, _ph2 )
  { }
  
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
  
  std::shared_ptr<hit> _ph1, _ph2;
  hitlist_t<params> _hitlist;
};

#include "hitlist_impl.inl"
