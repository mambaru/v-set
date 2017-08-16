#include "multiset_test_impl.hpp"

#include <vset/allocators/mmap_allocator.hpp>
#include <vset/allocators/buffer_allocator.hpp>
#include <vset/multiset.hpp>
#include <vset/memory/manager.hpp>
#include <vset/memory/fsb/aspect.hpp>

#define TEST_COUNT 10000

namespace {

struct data
{
  int data1;
  int data2;
  int data3;
};

typedef vset::memory::strategy::fsb_mmap<data> mmap_data_aspect;
typedef vset::memory::manager<mmap_data_aspect> data_buffer;
typedef data_buffer::const_pointer data_const_pointer;
typedef data_buffer::pointer data_pointer;

typedef unsigned int offset_t;

struct cmp123
{
  mutable data_const_pointer left;
  mutable data_const_pointer right;

  cmp123(): left(), right() {}

  cmp123(const data_buffer& b)
    : left(b.end())
    , right(b.end())
  {}

  bool operator()(offset_t l, offset_t r) const
  {
    left.set_offset(l);
    right.set_offset(r);

    return left->data1 < right->data1 ||
    ( ! ( right->data1 < left->data1 ) && left->data2 < right->data2 ) ||
    ( ! ( right->data1 < left->data1 ) && ! ( right->data2 < left->data2 ) && left->data3 < right->data3 );

    return false;
  }
};

typedef vset::multiset< offset_t, cmp123, vset::buffer_allocator<9> > index123_type;

data generate()
{
  data d;
  d.data1 = std::rand()%(TEST_COUNT*10);
  d.data2 = std::rand()%(TEST_COUNT*10);
  d.data3 = std::rand()%(TEST_COUNT*10);
  return d;
}

bool create(data_buffer& buffer, index123_type& index123)
{
  data_pointer ptr = buffer.allocate(1);
  *ptr = generate();
  index123_type::iterator itr = index123.find( ptr.get_offset() );
  if (itr == index123.end())
  {
    index123.insert( ptr.get_offset() );
  }
  else
  {
    buffer.deallocate(ptr, 1);
  }
  return itr == index123.end();
}

void create_one(data_buffer& buffer, index123_type& index123)
{
  while(!create(buffer,index123));
}

bool check(data_buffer& buffer, index123_type& index123)
{
  std::ptrdiff_t buffer_size = std::distance(buffer.begin(), buffer.end());
  std::ptrdiff_t buffer2_size = std::ptrdiff_t(buffer.count());
  std::ptrdiff_t index_size = std::ptrdiff_t(index123.size());
  std::ptrdiff_t index2_size = std::distance(index123.begin(), index123.end());
  bool size_check_fail = (buffer_size != index_size || buffer2_size != index2_size || buffer2_size != index_size);
  if ( size_check_fail )
  {
    std::cout << std::endl << "buffer/index size check failed!" << std::endl;
    std::cout << "buffer_size " << buffer_size << std::endl;
    std::cout << "index_size " << index_size << std::endl;
    return false;
  }

  if ( index123.size() < 2 )
  {
    return true;
  }
  
  cmp123 cmp(buffer);
  index123_type::iterator itr1 = index123.begin();
  index123_type::iterator itr2 = itr1 + 1;
  for ( ;itr2!=index123.end(); ++itr1, ++itr2)
  {
    if ( cmp(*itr1, *itr2) )
    {
      continue;
    }
    if ( !cmp(*itr2, *itr1) )
    {
      continue;
    }

    std::cout << std::endl << "comparator failed!" << std::endl;
    return false;
  }
  return true;
}

bool init(data_buffer& buffer, index123_type& index123)
{
  for (int i = 0; i < TEST_COUNT; )
  {
    if ( create(buffer, index123) )
    {
      ++i;
      if (i % 1000 == 0)
      {
        buffer.buffer().reserve( buffer.buffer().size() + 100);
        if( !check(buffer, index123) )
        {
          return false;
        }
      }
    }
  }
  return true;
}

bool erase_one(data_buffer& buffer, index123_type& index123)
{
  std::ptrdiff_t buffer_size = std::distance(buffer.begin(), buffer.end());
  data_pointer ptr = buffer.begin() + (buffer_size > 0 ? rand()%buffer_size : 0);
  offset_t offset = ptr.get_offset();
  
  index123_type::iterator lower = index123.lower_bound(offset);
  index123_type::iterator upper = index123.upper_bound(offset);
  if (std::distance(lower,upper)!=1 )
  {
    std::cout << ptr->data1 << "," << ptr->data2 << ", " << ptr->data3 << std::endl;
    std::cout << "std::distance(lower,upper): "  << std::distance(lower,upper) << std::endl;
    return false;
  }
  
  index123.erase( offset );
  buffer.deallocate(ptr, 1);
  std::ptrdiff_t buffer_size2 = std::distance(buffer.begin(), buffer.end());
  if ( (buffer_size - buffer_size2) != 1)
  {
    std::cout  << "Buffer size check failed - " << (buffer_size-1) << "!=" << buffer_size2 << std::endl;
    return false;
  }
  
  return true;
}

bool erase_begin(data_buffer& buffer, index123_type& index123)
{
  data_pointer ptr = buffer.begin();
  index123_type::iterator itr = index123.find( ptr.get_offset() );
  index123.erase(itr);
  buffer.deallocate(ptr, 1);
  return true;
}

bool clear(data_buffer& buffer, index123_type& index123)
{
  for (int i = 0; i < TEST_COUNT; )
  {
    if ( erase_one(buffer, index123) )
    {
      ++i;
      if (i%1000 == 0)
      {
        if( !check(buffer, index123) )
        {
          return false;
        }
      }
    }
  }

  return true;
}

bool stress(data_buffer& buffer, index123_type& index123, int count)
{
  for(int i =0 ; i < count && index123.size() > 1; i++)
  {
    erase_one(buffer, index123);
    if (i % 100 == 0)
    {
      if( !check(buffer, index123) )
      {
        return false;
      }
    }
    create_one(buffer, index123);
    if (i % 100 == 0)
    {
      if( !check(buffer, index123) )
      {
        return false;
      }
    }
  }

  return true;
}
}

bool multiset_test()
{
#if ( ! (__GNUC__==4 && __GNUC_MINOR__==6) )
  data_buffer buffer;
  buffer.buffer().open("./test2_.bin");
  buffer.buffer().reserve(TEST_COUNT*sizeof(data)+TEST_COUNT);
  buffer.buffer().clear();

  index123_type index123( (cmp123(buffer)) );
  index123.clear();

  return
    init(buffer, index123)
    
    && check(buffer, index123)
    && stress(buffer, index123, 10000)
    && check(buffer, index123)
    && clear(buffer, index123)
    && init(buffer, index123)
    && erase_begin(buffer, index123)
    && check(buffer, index123);
#else
  return true;
#endif
  
}
