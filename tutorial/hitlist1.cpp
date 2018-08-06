#include "hitlist.hpp"
#include <hitlist/compare.hpp>
#include <set>
#include <iostream>
#include <limits>

class hitlist::impl
{
  typedef std::multiset<hit, cmp_by_src> by_src_t;
  typedef std::multiset<hit, cmp_by_dst> by_dst_t;
  typedef std::multiset<hit, cmp_by_ts>  by_ts_t;
public:
  impl() { }
  
  bool open()
  {
    // TODO:
    return false;
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
    
    _by_src.insert( itr, h );
    _by_dst.insert( h );
    _by_ts.insert(  h );
  }
  
  bool delete_user(uint32_t id)
  {
    return delete_user_src_hits_(id) || delete_user_dst_hits_(id);
    /*
    hit h = hit::make(id, 0, static_cast<time_t>(~0));
    auto lower = _by_src.lower_bound( h );
    h.dst_id = static_cast<uint32_t>(~0);
    auto upper = _by_src.upper_bound( h );
    for (;lower!=upper; ++lower)
    {
      _by_dst.erase(*lower);
      _by_ts.erase(*lower);
      _by_src.erase(lower);
    }*/
    /*if ( lower!=upper )
    {
      _by_dst.erase(lower, upper);
      _by_ts.erase(lower, upper);
      _by_src.erase(lower, upper);
    }*/
    
    return true;
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
    hit h;
    h.dst_id = id;
    h.ts = std::numeric_limits<time_t>::max();
    auto lower = _by_dst.lower_bound(h);
/*    if ( lower == _by_dst.end() )
      return;*/
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
    return _by_src.size();
  }

  size_t capacity() const
  {
    /*
    _Rb_tree_color      _M_color;
    _Base_ptr           _M_parent;
    _Base_ptr           _M_left;
    _Base_ptr           _M_right;
    _Val _M_value_field
    */

    //return sizeof(std::_Rb_tree_node<hit>);
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
    hit h = hit::make(id, 0, std::numeric_limits<time_t>::max());
    auto lower = _by_src.lower_bound( h );
    h.dst_id = std::numeric_limits<uint32_t>::max();
    auto upper = _by_src.upper_bound( h );
    if ( lower==upper )
      return false;
    
    for (;lower!=upper; ++lower)
    {
      _by_dst.erase(*lower);
      _by_ts.erase(*lower);
      _by_src.erase(lower);
    }
    /*_by_dst.erase(lower, upper);
    _by_ts.erase(lower, upper);
    _by_src.erase(lower, upper);
    */
    
    return true;
  }

  bool delete_user_dst_hits_(uint32_t id)
  {
    hit h = hit::make(0, id, std::numeric_limits<time_t>::max());
    auto lower = _by_dst.lower_bound( h );
    h.src_id = std::numeric_limits<uint32_t>::max();
    auto upper = _by_dst.upper_bound( h );
    if ( lower==upper )
      return false;
    
    for (;lower!=upper; ++lower)
    {
      _by_src.erase(*lower);
      _by_ts.erase(*lower);
      _by_dst.erase(lower);
    
    }

    /*
    _by_ts.erase(lower, upper);
    _by_src.erase(lower, upper);
    _by_dst.erase(lower, upper);
    */
    
    return true;
  }

private:
  by_src_t _by_src;
  by_dst_t _by_dst;
  by_ts_t  _by_ts;
};

#include "hitlist_impl.hpp"
