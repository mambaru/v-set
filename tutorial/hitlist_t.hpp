#pragma once
#include <functional>
#include <iostream>
#include <algorithm>

template<typename T>
class hitlist_t
{
  typedef typename T::storage_type  storage_t;
  typedef typename T::src_index     by_src_t;
  typedef typename T::dst_index     by_dst_t;
  typedef typename T::ts_index      by_ts_t;
  //typedef typename T::pointer pointer;
  
  typedef typename storage_t::value_type value_type;
  typedef typename storage_t::pointer pointer;
  typedef typename by_src_t::value_type index_type;
  
  typedef typename by_src_t::iterator iterator;
  typedef typename std::iterator_traits<iterator>::iterator_category iterator_category;

public:
  
  hitlist_t(storage_t& stg, by_src_t& by_src, by_dst_t& by_dst, by_ts_t& by_ts, pointer& p1, pointer& p2)
    : _storage(stg)
    , _by_src(by_src)
    , _by_dst(by_dst)
    , _by_ts(by_ts)
    , _p1(p1)
    , _p2(p2)
  {
  }
  
  void set_hit(uint32_t src, uint32_t dst, time_t ts) 
  {
    pointer phit = _storage.allocate(1);
    *phit = hit::make(src, dst, ts);
    index_type ihit = get_index_(phit);
    auto itr = _by_src.find( ihit );
    if ( itr != _by_src.end() )
    {
      pointer ohit = get_ptr_(*itr, _storage);
      _by_dst.erase(*itr);
      _by_ts.erase(*itr);
      _by_src.erase(itr);
      _storage.deallocate(ohit, 1);
    }
    _by_src.insert( itr, ihit );
    _by_dst.insert( ihit );
    _by_ts.insert(  ihit );
  }
  
  size_t delete_user(uint32_t id)
  {
    return delete_user_src_(id, iterator_category() ) + delete_user_dst_(id, iterator_category() );
  }

  size_t remove_outdated(time_t ts)
  {
    return delete_user_ts_(ts, iterator_category() );
  }
  
  void get_hits( std::vector<hit>& hits, uint32_t dst_id, size_t offset, size_t limit) const
  {
    *_p1 = hit::make_lower_dst(dst_id);
    *_p2 = hit::make_upper_dst(dst_id);

    auto lower = _by_dst.lower_bound( get_index_(_p1) );
    auto upper = _by_dst.upper_bound( get_index_(_p2) );
    if (lower==upper)
      return;

    for(;lower!=upper && offset!=0; ++lower, --offset);
    for(;lower!=upper && limit!=0; ++lower, --limit)
    {
      hits.push_back( get_value_(*lower, _storage) );
    }
  }
  
  // сколько просмотрел пользователь с идентификатором id
  size_t src_count(uint32_t id) const
  {
    *_p1 =  hit::make_lower_src(id);
    *_p2 =  hit::make_upper_src(id);
    auto lower = _by_src.lower_bound( get_index_(_p1) );
    auto upper = _by_src.upper_bound( get_index_(_p2) );
    return static_cast<size_t>(std::distance(lower, upper));
  }
  
  // сколько было просмотров у пользователя с идентификатором id
  size_t dst_count(uint32_t id) const
  {
    *_p1 = hit::make_lower_dst(id);
    *_p2 = hit::make_upper_dst(id);
    auto lower = _by_dst.lower_bound( get_index_(_p1) );
    auto upper = _by_dst.upper_bound( get_index_(_p2) );
    return static_cast<size_t>(std::distance(lower, upper));
  }
  
  // сколько просмотров было позже ts
  size_t outdated_count(time_t ts) const
  {
    *_p1 = hit::make(0, 0, ts);
    auto lower = _by_ts.lower_bound( get_index_(_p1) );
    return static_cast<size_t>( std::distance(lower, _by_ts.end() ) );
  }
  
  void for_each(std::function<void(hit h)> handler)
  {
    for ( auto i : _by_src )
      handler( get_value_(i, _storage) );
  }

  size_t size() const 
  {
    return _by_src.size();
  }
  
private:

  
  /*value_type& get_ref_(pointer val) const  { return T::get_ref(val); }
  const value_type& get_ref_(const_pointer val) const  { return T::get_ref(val); }*/
  index_type get_index_(pointer p) const { return T::get_index(p); }
  pointer get_ptr_(index_type i, storage_t& stg) const { return T::get_pointer(i, stg); }
  hit get_value_(index_type i, storage_t& stg) const { return *get_ptr_(i, stg); }
  
  template<typename I1, typename I2, typename I3>
  size_t delete_user_t_(hit h1, hit h2, I1& i1, I2& i2, I3& i3, std::bidirectional_iterator_tag)
  {
    size_t count = 0;
    *_p1 = h1;
    *_p2 = h2;
    
    auto lower = i1.lower_bound( get_index_(_p1) );
    auto upper = i1.upper_bound( get_index_(_p2) );

    for (;lower!=upper; )
    {
      ++count;
      pointer p = get_ptr_(*lower, _storage);
      i2.erase(*lower);
      i3.erase(*lower);
      i1.erase(lower++);
      _storage.deallocate(p, 1);
    }
    return count;
  }
  
  template<typename I1, typename I2, typename I3>
  size_t delete_user_t_(hit h1, hit h2, I1& i1, I2& i2, I3& i3, std::random_access_iterator_tag)
  {
    size_t count = 0;
    *_p1 = h1;
    *_p2 = h2;
    auto lower = i1.lower_bound( get_index_(_p1) );
    auto upper = i1.upper_bound( get_index_(_p2) );
    if ( size_t dist = static_cast<size_t>(std::distance(lower, upper)) )
    {
      using namespace std::placeholders;
      std::vector<pointer> vect;
      vect.resize( size_t(dist), _p1 );
      count+=dist;
      std::transform(lower, upper, vect.begin(), std::bind(&hitlist_t<T>::get_ptr_, this, _1, _storage) );
      for (auto itr = lower; itr != upper; ++itr)
      {
        i2.erase(*itr);
        i3.erase(*itr);
      }
      i1.erase(lower, upper);
      std::for_each(vect.begin(), vect.end(), std::bind( &storage_t::deallocate, _storage, _1, 1) );
    }
    return count;
  }
  
  template<typename Tag>
  size_t delete_user_src_(uint32_t id, Tag)
  {
    return delete_user_t_(
      hit::make_lower_src(id), 
      hit::make_upper_src(id),
      _by_src, _by_dst, _by_ts, 
      Tag()
    );
  }
  
  template<typename Tag>
  size_t delete_user_dst_(uint32_t id, Tag)
  {
    return delete_user_t_(
      hit::make_lower_dst(id), 
      hit::make_upper_dst(id),
      _by_dst, _by_src, _by_ts, 
      Tag()
    );
  }
  
  template<typename Tag>
  size_t delete_user_ts_(time_t ts, Tag)
  {
    return delete_user_t_(
      hit::make(0, 0, ts), 
      hit::make(0, 0, 0),
      _by_ts, _by_dst, _by_src,
      Tag()
    );
  }
  
private:
  storage_t& _storage;
  by_src_t&  _by_src;
  by_dst_t&  _by_dst;
  by_ts_t&   _by_ts;
  pointer& _p1;
  pointer& _p2;
};


