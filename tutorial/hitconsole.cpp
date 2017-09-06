#include <iostream>
#include <hitlist/compare.hpp>
#include <vset/memory/fsb_inmem.hpp>
#include <vset/multiset.hpp>
#include <vset/comparators/offset_compare.hpp>

typedef size_t offset_t;

class hitlist
{
  typedef vset::memory::fsb_inmem<hit> storage_type;
  typedef vset::offset_compare<offset_t, storage_type, cmp_by_src> cmp_by_src_offs;
  typedef vset::offset_compare<offset_t, storage_type, cmp_by_dst> cmp_by_dst_offs;
  typedef vset::offset_compare<offset_t, storage_type, cmp_by_ts> cmp_by_ts_offs;
public:
  hitlist()
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
  
  std::array<size_t, 4> sizes() const
  {
    return {
      _storage.count(),
      _by_src.size(),
      _by_dst.size(),
      _by_ts.size()
    };
  }
private:
  storage_type::pointer _var;
  storage_type _storage;
  vset::multiset<offset_t, cmp_by_src_offs > _by_src;
  vset::multiset<offset_t, cmp_by_dst_offs > _by_dst;
  vset::multiset<offset_t, cmp_by_ts_offs >  _by_ts;
};

int main(int, char*[])
{
  hitlist hl;
  for (uint32_t i=0; i < 1000; ++i)
    hl.set_hit(i,i, time_t(i) );
  
  auto ss = hl.sizes();
  std::cout << std::get<0>(ss) << std::endl;
  std::cout << std::get<1>(ss) << std::endl;
  std::cout << std::get<2>(ss) << std::endl;
  std::cout << std::get<3>(ss) << std::endl;
  return 1;
}
