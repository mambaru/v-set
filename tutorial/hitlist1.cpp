#include "hitlist.hpp"
#include <hitlist/compare.hpp>
#include <set>
#include <iostream>
#include <limits>

template<typename T>
class hitlist_t
{
  typedef typename T::hit_type  hit_t;
  typedef typename T::src_index by_src_t;
  typedef typename T::dst_index by_dst_t;
  typedef typename T::ts_index  by_ts_t;
public:
  
  hitlist_t(by_src_t& by_src, by_dst_t& by_dst, by_ts_t& by_ts)
    : _by_src(by_src)
    , _by_dst(by_dst)
    , _by_ts(by_ts)
  {
  }
  
  void set_hit(const hit_t& h) 
  {
    auto itr = _by_src.find( h );
    if ( itr != _by_src.end() )
    {
      _by_dst.erase(*itr);
      _by_ts.erase(*itr);
      _by_src.erase(itr);
    }
    
    _by_src.insert( itr, h );
    _by_dst.insert( h );
    _by_ts.insert(  h );
  }
  
  size_t delete_user_src(const hit_t& from, const hit_t& to)
  {
    auto lower = _by_src.lower_bound( from );
    auto upper = _by_src.upper_bound( to );
    if ( lower==upper )
      return 0;
    
    size_t count = 0;
    for (;lower!=upper; ++lower)
    {
      ++count;
      _by_dst.erase(*lower);
      _by_ts.erase(*lower);
      _by_src.erase(lower);
    }
    
    return count;
  }

  size_t delete_user_dst(const hit_t& from, const hit_t& to)
  {
    auto lower = _by_dst.lower_bound( from );
    auto upper = _by_dst.upper_bound( to );
    if ( lower==upper )
      return 0;
    
    size_t count = 0;
    for (;lower!=upper; ++lower)
    {
      ++count;
      _by_ts.erase(*lower);
      _by_src.erase(*lower);
      _by_dst.erase(lower);
    }
    
    return count;
  }

  void get_hits( const hit_t& from, const hit_t& to, size_t offset, size_t limit, std::function<void(const hit_t&)> handler) const
  {
    auto lower = _by_dst.lower_bound(from);
    auto upper = _by_dst.upper_bound(to);
    for(;lower!=upper && offset!=0; ++lower, --offset);
    for(;lower!=upper && limit!=0; ++lower, --limit)
      handler(*lower);
    
    /*
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
    */
  }


  
private:
  by_src_t& _by_src;
  by_dst_t& _by_dst;
  by_ts_t&  _by_ts;
};


class hitlist::impl
{
  struct params
  {
    typedef hit hit_type;
    typedef std::multiset<hit, cmp_by_src> src_index;
    typedef std::multiset<hit, cmp_by_dst> dst_index;
    typedef std::multiset<hit, cmp_by_ts>  ts_index;
  };
  
  typedef params::src_index by_src_t;
  typedef params::dst_index by_dst_t;
  typedef params::ts_index by_ts_t;
public:
  
  impl()
    : _indexes(_by_src, _by_dst, _by_ts)
  { }
  
  void open() { }
  
  void set_hit(uint32_t src, uint32_t dst, time_t ts) 
  {
    _indexes.set_hit( hit::make(src, dst, ts) );
  }
  
  size_t delete_user(uint32_t id)
  {
    size_t count = 0;
    count += _indexes.delete_user_src(
      hit::make(id, 0, std::numeric_limits<time_t>::max()), 
      hit::make(id, std::numeric_limits<uint32_t>::max(), std::numeric_limits<time_t>::max())
    );

    count += _indexes.delete_user_dst(
      hit::make(0, id, std::numeric_limits<time_t>::max()), 
      hit::make(0, id, 0)
    );
    return count;
  }

  template<typename Itr>
  const hit& get(Itr itr) const
  {
    return *itr;
  }

  std::string desc() const
  {
    return "std::multiset<hit>";
  }
  
  void get_hits( std::vector<hit>& hits, uint32_t id, size_t offset, size_t limit) const
  {
    hits.clear();
    hits.reserve(limit);
    _indexes.get_hits(
      hit::make(id, 0, std::numeric_limits<time_t>::max()),
      hit::make(id, 0, 0),
      offset, limit,
      [&hits](const hit& h) { hits.push_back(h); }
    );
    /*
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
    */
  }
  
  size_t size() const
  {
    return _by_src.size();
  }

  size_t capacity() const
  {
    return (32 + sizeof(hit)) * _by_src.size() * 3;
    
  }
  
  void remove_outdated(time_t ts)
  {
    hit h = hit::make(0, 0, ts);
    auto lower = _by_ts.lower_bound( h );
    _by_src.erase(lower, _by_ts.end() );
    _by_dst.erase(lower, _by_ts.end() );
    _by_ts.erase( lower, _by_ts.end() );
  }
  
  
  // сколько просмотрел пользователь с идентификатором id
  size_t src_count(uint32_t id) const
  {
    hit h = hit::make(id, 0, 0);
    auto lower = _by_src.lower_bound(h);
    h.dst_id = static_cast<uint32_t>(~0);
    auto upper = _by_src.lower_bound(h);
    return static_cast<size_t>(std::distance(lower, upper));
  }
  
  // сколько было просмотров у пользователя с идентификатором id
  size_t dst_count(uint32_t id) const
  {
    hit h = hit::make(0, id, static_cast<time_t>(~0));
    auto lower = _by_dst.lower_bound(h);
    h.ts = 0;
    auto upper = _by_dst.lower_bound(h);
    return static_cast<size_t>( std::distance(lower, upper) );
  }
  
  // сколько просмотров было позже ts
  size_t outdated_count(uint32_t ts) const
  {
    hit h = hit::make(0, 0, ts);
    auto lower = _by_ts.lower_bound( h );
    return static_cast<size_t>( std::distance(lower, _by_ts.end() ) );
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
  hitlist_t<params> _indexes;
};

#include "hitlist_impl.hpp"
