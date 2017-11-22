#include <iostream>
#include <hitlist/compare.hpp>
#include <vset/memory/fsb_inmem.hpp>
#include <vset/multiset.hpp>
#include <vset/comparators/offset_compare.hpp>

typedef size_t offset_t;

class hitlist
{
  typedef vset::memory::fsb_inmem<hit> storage_type;
public:
  hitlist()
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
private:
  storage_type::pointer _var;
  storage_type _storage;
  vset::multiset<offset_t, vset::offset_compare<offset_t, storage_type, cmp_by_src> > _by_src;
  vset::multiset<offset_t, vset::offset_compare<offset_t, storage_type, cmp_by_dst> > _by_dst;
  vset::multiset<offset_t, vset::offset_compare<offset_t, storage_type, cmp_by_ts> >  _by_ts;
};

int main(int, char*[])
{
  hitlist hl;
  hl.set_hit(0,0,0);
  return 1;
}
