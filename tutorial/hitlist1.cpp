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
    hit h = hit::make(src, dst, ts);
    
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
  
  bool delete_user(uint32_t id)
  {
    hit h = hit::make(id, 0, static_cast<time_t>(~0));
    auto lower = _by_src.lower_bound( h );
    h.dst_id = static_cast<uint32_t>(~0);
    auto upper = _by_src.upper_bound( h );
    if ( lower!=upper )
    {
      _by_dst.erase(lower, upper);
      _by_ts.erase(lower, upper);
      _by_src.erase(lower, upper);
    }
    
    return true;
  }

  template<typename Itr>
  const hit& get(Itr itr) const
  {
    return *itr;
  }

  std::string desc() const
  {
    return "";
  }
  
  void get_hits( std::vector<hit>& hits, uint32_t id, size_t offset, size_t limit) const
  {
    hits.clear();
    hits.reserve(limit);
    hit h;
    h.dst_id = id;
    h.ts = ~0;
    auto lower = _by_dst.lower_bound(h);
    h.ts = 0;
    auto upper = _by_dst.upper_bound(h);
    for(;lower!=upper && offset!=0; ++lower, --offset);
    for(;lower!=upper && limit!=0; ++lower, --limit)
    {
      hits.push_back( this->get(lower) );
    }
  }
  
  size_t size() const
  {
    return 0;
  }

  size_t capacity() const
  {
    return 0;
  }
  
  size_t remove_outdated(time_t )
  {
    return 0;
  }
  
  total get_total(uint32_t ) const
  {
    return total();
  }

  
private:
  
  bool delete_user_src_hits_(uint32_t id)
  {
    hit h = hit::make(id, 0, static_cast<time_t>(~0));
    auto lower = _by_src.lower_bound( h );
    h.dst_id = static_cast<uint32_t>(~0);
    auto upper = _by_src.upper_bound( h );
    if ( lower==upper )
      return false;
    
    _by_dst.erase(lower, upper);
    _by_ts.erase(lower, upper);
    _by_src.erase(lower, upper);
    
    return true;
  }

  bool delete_user_dst_hits_(uint32_t id)
  {
    hit h = hit::make(0, id, static_cast<time_t>(~0));
    auto lower = _by_dst.lower_bound( h );
    h.src_id = static_cast<uint32_t>(~0);
    auto upper = _by_dst.upper_bound( h );
    if ( lower==upper )
      return false;
    
    _by_ts.erase(lower, upper);
    _by_src.erase(lower, upper);
    _by_dst.erase(lower, upper);
    
    return true;
  }

private:
  by_src_t _by_src;
  by_dst_t _by_dst;
  by_ts_t  _by_ts;
};

#include "hitlist_impl.hpp"
