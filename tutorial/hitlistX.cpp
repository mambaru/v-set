#include "hitlist.hpp"
#include "hitlist_t.hpp"
#include <hitlist/compare.hpp>
#include <set>
#include <iostream>
#include <limits>
#include <vset/allocators/mmap_allocator.hpp>
#include <vset/memory/fsb_mmap.hpp>
#include <vset/comparators/offset_compare.hpp>
#include <vset/multiset.hpp>


class hitlist::impl
{
  const std::string name="hitlistX";
  typedef uint32_t offset_t;
  
  typedef vset::memory::fsb_mmap<hit, vset::fsb_nth> storage_type;
  typedef storage_type::pointer pointer_type;
  typedef storage_type::const_pointer const_pointer_type;
  typedef vset::offset_compare<offset_t, storage_type, hit_src_cmp> cmp_by_src_offs;
  typedef vset::offset_compare<offset_t, storage_type, hit_dst_cmp> cmp_by_dst_offs;
  typedef vset::offset_compare<offset_t, storage_type, hit_ts_cmp>  cmp_by_ts_offs;
  typedef vset::mmap_allocator<512> allocator_type;
  typedef vset::multiset<offset_t, cmp_by_src_offs, allocator_type > by_src_t;
  typedef vset::multiset<offset_t, cmp_by_dst_offs, allocator_type > by_dst_t;
  typedef vset::multiset<offset_t, cmp_by_ts_offs, allocator_type >  by_ts_t;

  struct params
  {
    typedef ::hitlist::impl::storage_type storage_type;
    typedef by_src_t src_index;
    typedef by_dst_t dst_index;
    typedef by_ts_t  ts_index;
    
    static offset_t get_index(pointer_type p) 
    {
      return static_cast<offset_t>(p.get_offset());
    }
    
    static pointer_type get_pointer(offset_t offs, storage_type& stg) 
    {
      pointer_type p = stg.end();
      p.set_offset(offs);
      return p;
    }
    
    /*
    static hit& get_ref(pointer_type h) 
    {
      return *h;
    }
    
    static const hit& get_ref(const_pointer_type h) 
    {
      return *h;
    }
    */
  };
  
public:
  
  impl()
    : _storage()
    , _by_src( cmp_by_src_offs(_storage.end()))
    , _by_dst( cmp_by_dst_offs(_storage.end()))
    , _by_ts( cmp_by_ts_offs(_storage.end()))
    , _hitlist(_storage, _by_src, _by_dst, _by_ts, _hit1, _hit2)
  { 
  }
  
  bool open(size_t reserve1, size_t reserve2) 
  {
    std::string filename = name+".stg";
    _storage.buffer().open( filename.c_str() );
    bool is_empty = false;
    if ( _storage.empty() )
    {
      _hit1=_storage.allocate(1);
      _hit2=_storage.allocate(1);
      _storage.buffer().reserve( reserve1 );
      is_empty = true;
    }
    else
    {
      _hit1=_storage.begin();
      _hit2=_storage.begin()+1;
    }
    filename = name+".idxs";
    _by_src.get_allocator().memory().buffer().open(filename.c_str());
    if ( is_empty) _by_src.get_allocator().memory().buffer().reserve(reserve2);
    filename = name+".idxd";
    _by_dst.get_allocator().memory().buffer().open(filename.c_str());
    if ( is_empty) _by_dst.get_allocator().memory().buffer().reserve(reserve2);
    filename = name+".idxt";
    _by_ts.get_allocator().memory().buffer().open(filename.c_str());
    if ( is_empty) _by_ts.get_allocator().memory().buffer().reserve(reserve2);
    std::cout << "storage size: " << _storage.count()  << " index(" << _by_src.size()  << ", "<< _by_dst.size() << "," << _by_ts.size() << ")" << std::endl;
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
  
  mutable pointer_type _hit1;
  mutable pointer_type _hit2;
};

#include "hitlist_impl.inl"
