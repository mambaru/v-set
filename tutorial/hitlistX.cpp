#include "hitlist.hpp"
#include <hitlist/compare.hpp>
#include <vset/memory/fsb_inmem.hpp>
#include <vset/multiset.hpp>
#include <vset/comparators/offset_compare.hpp>

typedef size_t offset_t;

class hitlist::impl
{
  typedef vset::memory::fsb_inmem<hit> storage_type;
  typedef vset::offset_compare<offset_t, storage_type, cmp_by_src> cmp_by_src_offs;
  typedef vset::offset_compare<offset_t, storage_type, cmp_by_dst> cmp_by_dst_offs;
  typedef vset::offset_compare<offset_t, storage_type, cmp_by_ts> cmp_by_ts_offs;
public:
  impl()
    : _by_src( cmp_by_src_offs( _storage.end() ) )
    , _by_dst( cmp_by_dst_offs( _storage.end() ) )
    , _by_ts ( cmp_by_ts_offs( _storage.end() ) )
  {
  }
  
  void open()
  {
    if ( _storage.empty() )
      _var = _storage.allocate(1);
    else
      _var = _storage.begin();
  }
  
  void set_hit(uint32_t src, uint32_t dst, time_t ts) 
  {
    auto h = _storage.allocate(1);
    *h = {src, dst, ts};
    
    auto itr = _by_src.find( h.get_offset() );
    if ( itr != _by_src.end() )
    {
      _by_dst.erase(*itr);
      _by_ts.erase(*itr);
      _by_src.erase(itr);
    }
    
    _by_src.insert( h.get_offset() );
    _by_dst.insert( h.get_offset() );
    _by_ts.insert( h.get_offset() );
  }
  
  template<typename Itr>
  const hit& get(Itr itr) const
  {
    auto ptr = _storage.end();
    ptr.set_offset(*itr);
    return *ptr;
  }

  
  size_t get_hits( std::vector<hit>& hits, uint32_t id, size_t offset, size_t limit) const
  {
    hits.clear();
    hits.reserve(limit);
    _var->dst_id = id;
    _var->ts = ~0;
    auto lower = _by_dst.lower_bound(_var.get_offset());
    _var->ts = 0;
    auto upper = _by_dst.upper_bound(_var.get_offset());
    auto dist = std::distance(lower, upper);
    for(;lower!=upper && offset!=0; ++lower, --offset);
    for(;lower!=upper && limit!=0; ++lower, --limit)
    {
      hits.push_back( this->get(lower) );
    }
    return static_cast<size_t>(dist);
  }
  
  size_t size() const
  {
    return 0;
  }

  size_t capacity() const
  {
    return 0;
  }
  
private:
  mutable storage_type::pointer _var;
  storage_type _storage;
  vset::multiset<offset_t, cmp_by_src_offs > _by_src;
  vset::multiset<offset_t, cmp_by_dst_offs > _by_dst;
  vset::multiset<offset_t, cmp_by_ts_offs >  _by_ts;
};

#include "hitlist_impl.hpp"
