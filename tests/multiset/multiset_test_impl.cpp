#include "multiset_test_impl.hpp"

#include <vset/allocators/mmap_allocator.hpp>
#include <vset/multiset.hpp>
#include <vset/memory/manager.hpp>
#include <vset/memory/fsb/aspect.hpp>

#define TEST_COUNT 10000


struct data
{
  int data1;
  int data2;
  int data3;
};

typedef vset::memory::fsb::aspect<data> mmap_data_aspect;
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
    left = static_cast<size_t>(l);
    right = static_cast<size_t>(r);

    
    if ( left->data1 < right->data1 )
      return true;

    if ( right->data1 < left->data1 )
      return false;

    if ( left->data2 < right->data2 )
      return true;

    if ( right->data2 < left->data2 )
      return false;

    if ( left->data3 < right->data3 )
      return true;

    if ( right->data3 < left->data3 )
      return false;

    return false;
  }
};

typedef vset::multiset< offset_t, cmp123, vset::mmap_allocator<13> > index123_type;

data generate()
{
  return data{ std::rand()%(TEST_COUNT/10), std::rand()%(TEST_COUNT/10), std::rand()%(TEST_COUNT/10) };
}

bool create(data_buffer& buffer, index123_type& index123)
{
  data_pointer ptr = buffer.allocate(1);
  *ptr = generate();
  auto itr = index123.find(static_cast<offset_t>( static_cast<size_t>(ptr) ));
  if (itr == index123.end())
    index123.insert( static_cast<offset_t>( static_cast<size_t>(ptr) ) );
  else
    buffer.deallocate(ptr, 1);
  return itr == index123.end();
  return true;
}

void check(data_buffer& buffer, index123_type& index123)
{
  size_t buffer_size = std::distance(buffer.begin(), buffer.end());
  size_t index_size = index123.size();
  if (buffer_size != index_size)
  {
    std::cout << "buffer_size " << buffer_size << std::endl;
    std::cout << "index_size " << index_size << std::endl;
    abort();
  }

  if ( index123.size() < 2 )
    return;
  
  cmp123 cmp(buffer);
  index123_type::iterator itr1 = index123.begin();
  index123_type::iterator itr2 = itr1 + 1;
  // ++itr2;
  for ( ;itr2!=index123.end(); ++itr1, ++itr2)
  {
    if ( cmp(*itr1, *itr2) )
      continue;
    if ( !cmp(*itr2, *itr1) )
      continue;

    std::cout << "comp fail" << std::endl;
    abort();
  }
  
}

void init(data_buffer& buffer, index123_type& index123)
{
  std::cout << "init" << std::endl;
  for (int i = 0; i < TEST_COUNT; )
  {
    if ( create(buffer, index123) )
    {
      ++i;
      if (i%100 == 0)
      {
        std::cout << "create\t" << i << std::endl;
        check(buffer, index123);
      }
    }
  }

  std::cout << index123.size() << std::endl;
}

bool erase(data_buffer& buffer, index123_type& index123)
{
  //std::cout << "erase1" << std::endl;
  size_t buffer_size = std::distance(buffer.begin(), buffer.end());
  // std::cout << "erase2" << std::endl;
  data_pointer ptr = buffer.begin() + rand()%buffer_size;

  /*
  auto lower = index123.lower_bound(static_cast<offset_t>( static_cast<size_t>(ptr) ));
  auto upper = index123.upper_bound(static_cast<offset_t>( static_cast<size_t>(ptr) ));
  if (std::distance(lower,upper)!=1 )
  {
    std::cout << ptr->data1 << "," << ptr->data2 << ", " << ptr->data3 << std::endl;
    std::cout << "std::distance(lower,upper): "  << std::distance(lower,upper) << std::endl;
    abort();
  }
  index123.erase(lower, upper);
  buffer.deallocate(ptr, 1);
  */
  
  /*auto itr = index123.find(static_cast<offset_t>( static_cast<size_t>(ptr) ));
  index123.erase(itr);
  buffer.deallocate(ptr, 1);
  */
  
  
  offset_t offset = static_cast<offset_t>( static_cast<size_t>(ptr) );
  //std::cout << "erase3 " << offset << "<?" << buffer_size << std::endl;
  index123.erase( offset );
  // std::cout << "erase4" << std::endl;
  buffer.deallocate(ptr, 1);
  // std::cout << "erase5" << std::endl;
  size_t buffer_size2 = std::distance(buffer.begin(), buffer.end());
  // std::cout << "erase6" << std::endl;
  if ( (buffer_size - buffer_size2) != 1)
  {
    std::cout  << "fuck " << (buffer_size-1) << "!=" << buffer_size2 << std::endl;
    abort();
  }
  
  return true;
}

/*
bool erase_range(data_buffer& buffer, index123_type& index123)
{
  size_t buffer_size = std::distance(buffer.begin(), buffer.end());
  if ( buffer_size < 10)
    return erase(buffer, index123)
  data_pointer ptr = buffer.begin() + rand()%buffer_size;
  auto itr = index123.find(static_cast<offset_t>( static_cast<size_t>(ptr) ));
  index123.erase(itr);
  buffer.deallocate(ptr, 1);
  size_t buffer_size2 = std::distance(buffer.begin(), buffer.end());
  if ( (buffer_size - buffer_size2) != 1)
  {
    std::cout  << "fuck " << (buffer_size-1) << "!=" << buffer_size2 << std::endl;
    abort();
  }
  return true;
}
*/

bool erase_begin(data_buffer& buffer, index123_type& index123)
{
  data_pointer ptr = buffer.begin();
  auto itr = index123.find(static_cast<offset_t>( static_cast<size_t>(ptr) ));
  index123.erase(itr);
  buffer.deallocate(ptr, 1);
  return true;
}


void clear(data_buffer& buffer, index123_type& index123)
{
  std::cout << "clear" << std::endl;
  for (int i = 0; i < TEST_COUNT; )
  {
    // std::cout << "erase i=" << i << std::endl;
    if ( erase(buffer, index123) )
    {
      ++i;
      //if (i%100 == 0)
      {
        std::cout << "erase\t" << TEST_COUNT - i << std::endl;
        check(buffer, index123);
      }
    }
  }

  std::cout << index123.size() << std::endl;
}

bool multiset_test()
{
  //std::srand( time(0) );
  std::cout << "multiset_test()" << std::endl;
  data_buffer buffer;
  std::cout << "open..." << std::endl;
  buffer.buffer().open("./test2_.bin");
  std::cout << "reserve..." << std::endl;
  buffer.buffer().reserve(TEST_COUNT*sizeof(data));

  //cmp123 cmp = cmp123(buffer);
  index123_type index123( (cmp123(buffer)) );
  std::cout << "open..." << std::endl;
  index123.get_allocator().memory().buffer().open("./test2_index123.bin");
  index123.get_allocator().memory().buffer().reserve(TEST_COUNT*sizeof(data)*2);

  init(buffer, index123);
  clear(buffer, index123);
  check(buffer, index123);
  return true;
}