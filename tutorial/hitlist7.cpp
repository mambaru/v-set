#include "hitlist.hpp"
#include "hitlist_t.hpp"
#include <hitlist/compare.hpp>
#include <set>
#include <iostream>
#include <limits>
#include <vset/allocators/inmem_allocator.hpp>
#include <vset/memory/fsb_mmap.hpp>
#include <vset/comparators/offset_compare.hpp>
#include <vset/multiset.hpp>


class hitlist::impl
{
  const std::string name="hitlist7";
  
  typedef vset::memory::fsb_mmap<hit, vset::fsb_nth> storage_type;
  typedef storage_type::pointer pointer_type;
  typedef storage_type::const_pointer const_pointer_type;
  typedef vset::inmem_allocator<512> allocator_type;
  typedef hit_idx_t<pointer_type> index_type;
  typedef vset::multiset<index_type, hit_src_cmp, allocator_type > by_src_t;
  typedef vset::multiset<index_type, hit_dst_cmp, allocator_type > by_dst_t;
  typedef vset::multiset<index_type, hit_dst_cmp, allocator_type >  by_ts_t;

  struct params
  {
    typedef ::hitlist::impl::storage_type storage_type;
    typedef by_src_t src_index;
    typedef by_dst_t dst_index;
    typedef by_ts_t  ts_index;
    
    static index_type get_index(pointer_type p) 
    {
      index_type idx;
      static_cast<hit&>(idx) = *p;
      idx.target = p;
      return idx;
    }
    
    static pointer_type get_pointer(index_type idx, storage_type& ) 
    {
      return idx.target;
    }
 };
  
public:
  
  impl()
    : _storage()
    , _hitlist(_storage, _by_src, _by_dst, _by_ts, _p1, _p2)
  { 
  }
  
  impl(const impl&) = delete;
  
  bool open(size_t reserve1, size_t /*reserve2*/) 
  {
    std::string filename = name+".stg";
    _storage.buffer().open( filename.c_str() );
    _storage.buffer().reserve(reserve1);
    auto beg = _storage.begin();
    auto end = _storage.end();
    for (;beg!=end; ++beg)
    {
      index_type idx;
      static_cast<hit&>(idx) = *beg;
      idx.target = beg;
      _by_src.insert(idx);
      _by_dst.insert(idx);
      _by_ts.insert(idx);
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
    return 0;
  }

  
#include "hitlist_methods.inl"

private:
  storage_type _storage;
  by_src_t _by_src;
  by_dst_t _by_dst;
  by_ts_t  _by_ts;
  hitlist_t<params> _hitlist;
  
  mutable pointer_type _p1;
  mutable pointer_type _p2;
};

#include "hitlist_impl.inl"
