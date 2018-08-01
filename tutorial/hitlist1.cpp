#include "hitlist.hpp"
#include <hitlist/compare.hpp>
#include <set>



class hitlist::impl
{
  typedef std::multiset<hit, cmp_by_src> by_src_t;
  typedef std::multiset<hit, cmp_by_dst> by_dst_t;
  typedef std::multiset<hit, cmp_by_ts>  by_ts_t;
public:
  impl()
  {
  }
  
  void open()
  {
    // TODO:
  }
  
  void set_hit(uint32_t src, uint32_t dst, time_t ts) 
  {
    hit h = {src, dst, ts};
    
    auto itr = _by_src.find( h );
    if ( itr != _by_src.end() )
    {
      _by_dst.erase(*itr);
      _by_ts.erase(*itr);
      _by_src.erase(itr);
    }
    
    _by_src.insert( h );
    _by_dst.insert( h );
    _by_ts.insert( h );
  }
  
  template<typename Itr>
  const hit& get(Itr itr) const
  {
    return *itr;
  }

  
  size_t get_hits( std::vector<hit>& hits, uint32_t id, size_t offset, size_t limit) const
  {
    hits.clear();
    hits.reserve(limit);
    hit h;
    h.dst_id = id;
    h.ts = ~0;
    auto lower = _by_dst.lower_bound(h);
    h.ts = 0;
    auto upper = _by_dst.upper_bound(h);
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
  by_src_t _by_src;
  by_dst_t _by_dst;
  by_ts_t  _by_ts;
};

#include "hitlist_impl.hpp"
