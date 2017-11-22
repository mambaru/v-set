//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/memory/manager.hpp>
#include <vset/memory/provider.hpp>
#include <vset/memory/allocator.hpp>
#include <vset/memory/fsb/aspect.hpp>
#include <vset/buffer/persistent/filesync/aspect.hpp>

size_t off2ptr(size_t off);
size_t ptr2off(size_t ptr);

const size_t CHAIN_HEAD = 16;
const size_t CHANK_HEAD = 8;
const size_t CHANK_COUNT = 64;
const size_t VALUE_SIZE = 8;
const size_t VALUE_BLOCK = VALUE_SIZE*64;
const size_t CHANK_SIZE = CHANK_HEAD + VALUE_BLOCK ;

size_t off2ptr(size_t off)
{
  size_t result = CHAIN_HEAD;
  result += CHANK_HEAD + (off / CHANK_COUNT) * CHANK_HEAD;
  result += VALUE_SIZE*off;
  return result ;
}

size_t ptr2off(size_t ptr)
{
  size_t result = ptr - CHAIN_HEAD;
  result -= (result/CHANK_SIZE) * CHANK_HEAD;
  result -= CHANK_HEAD;
  return result/VALUE_SIZE;
}

UNIT(test_offset, "")
{
  using namespace fas::testing;
  // t << is_true<expect>(false) << FAS_FL;
  // t << stop;
  //return;
  for (size_t i = 0; i < 10000; ++i )
  {
    size_t ptr = off2ptr(i);
    size_t off = ptr2off(ptr);
    //t << message("i=") << i << " off=" << off << " ptr=" << ptr << " > " << (ptr - CHAIN_HEAD) / CHANK_SIZE ;
    t << equal<crash>(i, off) << " ptr=" << ptr <<  FAS_FL;
    t << stop;
    //return;
  }
}

#define MAX_TEST 777 
const size_t CAPACITY = (777/64)*64 + (777%64!=0)*64;

//typedef char value_type;

template<typename T, typename Alloc>
void test_char_init(T& t, Alloc& allocator)
{
  using namespace fas::testing;
  typedef typename Alloc::pointer pointer;
  typedef typename Alloc::value_type value_type;

  for (int i=0; i < MAX_TEST; ++i)
  {
    value_type* ch = allocator.allocate(1).get_address();
    t << is_true<assert>( ch != 0 ) << "i=" << i << " " << FAS_TESTING_FILE_LINE;
    t << stop;
    *ch = '0' + i%10;
  }

  pointer beg = allocator.begin();
  pointer end = allocator.end();

  int i=0;
  for ( ; beg != end; ++i)
  {
    //std::cout << "---------------------------" << std::endl;
    t << equal< assert, value_type > ( *beg, '0' + i%10 ) << char(*beg) << "!=" << char('0' + i%10) << " " << FAS_TESTING_FILE_LINE;
    size_t off1 = beg.get_offset();
    value_type* add1 = beg.get_address();
    t << equal< assert, value_type > ( *add1, '0' + i%10 ) << FAS_FL;
    t << equal< assert, value_type* > ( &(*beg), &(*add1) ) << FAS_FL;
    beg.set_offset(off1);
    t << equal< assert, size_t > ( off1, beg.get_offset() ) << FAS_FL;
    t << equal< assert, value_type* > ( add1, beg.get_address() ) << FAS_FL;
    size_t off2 = beg.set_address(add1);
    t << equal< assert, size_t > ( off1, off2 ) << FAS_FL;
    t << equal< assert, value_type* > ( add1, beg.get_address() ) << "[" << (add1 - beg.get_address()) << "]"<< FAS_FL;
    t << equal< assert, size_t > ( off1, beg.get_offset() ) << FAS_FL;
    t << stop;
    //std::cout << "next" << std::endl;
    ++beg;
  }
}

template<typename T, typename Alloc>
void test_char_test(T& t, const Alloc& allocator)
{
  using namespace fas::testing;
  typedef typename Alloc::const_pointer const_pointer;
  typedef typename Alloc::value_type value_type;

  const_pointer beg = allocator.begin();
  const_pointer end = allocator.end();
  int i=0;
  for ( ; beg != end; ++beg, ++i)
  {
    t << equal< assert, value_type > ( *beg, '0' + i%10 ) << char(*beg) << "!=" << char('0' + i%10) << " " << FAS_TESTING_FILE_LINE;
  }
  t << equal< assert, int > ( i, MAX_TEST) << i << "!=" << MAX_TEST << " " << FAS_TESTING_FILE_LINE;

  t << equal< assert, int > ( i, allocator.count() ) << FAS_TESTING_FILE_LINE;
  t << equal< assert, int > ( CAPACITY, allocator.capacity() ) << FAS_TESTING_FILE_LINE;

  beg = allocator.begin();
  const_pointer beg1 = beg;
  ++beg1;
  --beg1;
  
  t << equal< assert > ( *beg, *beg1 ) << FAS_TESTING_FILE_LINE;
  t << equal< assert > ( beg, beg1 ) << FAS_TESTING_FILE_LINE;
  
  
  i = MAX_TEST - 1;
  beg = allocator.begin();
  end = allocator.end();
  
  for ( --end; beg!=end;--end, --i)
  {
    if ( i < 0 )
    {
      t << is_true< assert>(false, "Iterator is out of range!!!");
      break;
    }

    t << equal< assert, value_type > ( *end, '0' + i%10 ) << char(*end) << "!=" << char('0' + i%10) << " " << FAS_TESTING_FILE_LINE;
  }
}

UNIT(test_unit, "")
{
  using namespace fas::testing;
  typedef vset::memory::manager< vset::memory::strategy::fsb_mmap<char, ::vset::memory::fsb::aspect_offset > > allocator_type;
  
  allocator_type allocator;
  allocator.buffer().open("allocator.bin");
  allocator.buffer().truncate(0);

  test_char_init(t, allocator);
  test_char_test(t, allocator);

  allocator.buffer().sync();
  allocator.buffer().close();
  allocator.buffer().open("allocator.bin");

  test_char_test(t, allocator);

  allocator_type allocator2;
  allocator2.buffer().open("allocator.bin");

  test_char_test(t, allocator);

  allocator2.buffer().close();
  allocator.buffer().close();

  t << nothing;
  
}

UNIT(test_allocator, "")
{
  using namespace fas::testing;
  using namespace vset;

  typedef memory::manager< vset::memory::strategy::fsb_mmap<char, ::vset::memory::fsb::aspect_offset > > manager;
  typedef memory::provider< manager > provider;
  typedef memory::allocator< provider > allocator;

  manager m;
  provider p(&m);
  allocator a( p );
  a.memory().buffer().open("allocator2.bin");
  a.memory().buffer().clear();
  *(a.allocate(1))='X';
  a.memory().buffer().sync();
  a.memory().buffer().close();
  t << nothing;
}

BEGIN_SUITE(basic_suite, "")
  ADD_UNIT(test_offset)
  ADD_UNIT(test_unit)
  ADD_UNIT(test_allocator)
END_SUITE(basic_suite)
