#include "hitlist.hpp"
#include "hitlist_t.hpp"
#include <hitlist/compare.hpp>
#include <vset/comparators/pointer_compare.hpp>
#include <vset/buffer/persistent_buffer.hpp>
#include <vset/buffer/strategy.hpp>

#include <set>
#include <iostream>
#include <limits>
#include <memory>

class hitlist::impl
{
  const std::string name="hitlist4";
  struct storage
  {
    typedef hit value_type;
    typedef hit* pointer;
    typedef const hit* const_pointer;

    pointer allocate(size_t)
    {
      if ( !_free.empty() )
      {
        pointer p = *(_free.begin());
        _free.erase(_free.begin());
        return p;
      }

      size_t pos = _buffer.size();
      _buffer.resize(pos + sizeof(value_type) );
      return static_cast<pointer>( static_cast<void*>(_buffer.data() + pos )) ;
    }

    void deallocate(pointer p, size_t)
    {
      *p = value_type();
      _free.insert(p);
    }

    typedef vset::buffer::persistent_buffer< vset::buffer::strategy::mmap > buffer_type;
    buffer_type& buffer() {return _buffer;}
  private:
    std::set<pointer> _free;
    buffer_type _buffer;
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
    : _p1(new value_type()), _p2(new value_type())
    , _hitlist(_storage, _by_src, _by_dst, _by_ts, _p1, _p2 )
  { }

  ~impl()
  {
    delete _p1;
    delete _p2;
  }

  impl(const impl&) = delete;

  bool open(size_t reserve1, size_t)
  {
    std::string filename = name+".stg";
    _storage.buffer().open( filename.c_str() );
    _storage.buffer().reserve(reserve1);

    if ( size_t s = _storage.buffer().size() )
    {
      pointer beg = static_cast<pointer>( static_cast<void*>(_storage.buffer().data()));
      pointer end = beg + s/sizeof(value_type);
      for (;beg!=end; ++beg)
      {
        if ( beg->src_id==0 && beg->dst_id==0 && beg->ts==0 )
        {
          _storage.deallocate(beg,1);
        }
        else
        {
          _by_src.insert(beg);
          _by_dst.insert(beg);
          _by_ts.insert(beg);
        }
      }
    }
    return true;
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

  pointer _p1;
  pointer _p2;

  hitlist_t<params> _hitlist;
};

#include "hitlist_impl.inl"
