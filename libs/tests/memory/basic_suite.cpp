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
//#include <vset/persistent_buffer.hpp>
#include <vset/buffer/persistent/filesync/aspect.hpp>

//#define MAX_TEST 6400
#define MAX_TEST 777 

const size_t CAPACITY = (777/64)*64 + (777%64!=0)*64;

typedef char value_type;

template<typename T, typename Alloc>
void test_char_init(T& t, Alloc& allocator)
{
  using namespace fas::testing;
  typedef typename Alloc::pointer pointer;

  for (int i=0; i < MAX_TEST; ++i)
  {
    value_type* ch = allocator.allocate(1);
    t << is_true<assert>( ch != 0 ) << "i=" << i << " " << FAS_TESTING_FILE_LINE;
    t << stop;
    *ch = '0' + i%10;
  }

  pointer beg = allocator.begin();
  pointer end = allocator.end();

  int i=0;
  for ( ;beg!=end;++beg, ++i)
    t << equal< assert, value_type > ( *beg, '0' + i%10 ) << char(*beg) << "!=" << char('0' + i%10) << " " << FAS_TESTING_FILE_LINE;

}

template<typename T, typename Alloc>
void test_char_test(T& t, const Alloc& allocator)
{
  using namespace fas::testing;
  typedef typename Alloc::const_pointer const_pointer;
  const_pointer beg = allocator.begin();
  const_pointer end = allocator.end();
  int i=0;
  for ( ;beg!=end;++beg, ++i)
    t << equal< assert, value_type > ( *beg, '0' + i%10 ) << char(*beg) << "!=" << char('0' + i%10) << " " << FAS_TESTING_FILE_LINE;
  t << equal< assert, int > ( i, MAX_TEST) << i << "!=" << MAX_TEST << " " << FAS_TESTING_FILE_LINE;

  std::cout << "COUNT=" <<  allocator.count() << std::endl;
  std::cout << "CAPACITY=" << CAPACITY << " " <<  allocator.capacity() << std::endl;
  std::cout << "i=" <<  i << std::endl;

  t << equal< assert, int > ( i, allocator.count() ) << FAS_TESTING_FILE_LINE;
  t << equal< assert, int > ( CAPACITY, allocator.capacity() ) << FAS_TESTING_FILE_LINE;

  beg = allocator.begin();
  const_pointer beg1 = beg;
  std::cout << "offset1=" << beg1.get_offset() << std::endl;
  ++beg1;
  std::cout << "offset2=" << beg1.get_offset() << std::endl;
  --beg1;
  std::cout << "offset3=" << beg1.get_offset() << std::endl;
  
  t << equal< assert > ( beg, beg1 ) << FAS_TESTING_FILE_LINE;
  t << equal< assert > ( *beg, *beg1 ) << FAS_TESTING_FILE_LINE;

  
  std::cout << "xxx i=" << "ready" << std::endl;
  i = MAX_TEST - 1;
  beg = allocator.begin();
  end = allocator.end();
  /*--end;

  std::cout << "### " << *end << ":" << char('0' + i%10) << std::endl;
  */
  
  //--end;
  
  for ( --end; beg!=end;--end, --i)
  {
    
    std::cout << "xxx i=" << i << std::endl;
    if ( i < 0 )
    {
      t << fatal( "FUCK" );
      break;
    }

    t << equal< assert, value_type > ( *end, '0' + i%10 ) << char(*end) << "!=" << char('0' + i%10) << " " << FAS_TESTING_FILE_LINE;
    
  }

  
  /*
  std::reverse_iterator<const_pointer> rbeg(allocator.end());
  std::reverse_iterator<const_pointer> rend(allocator.begin());

  i = MAX_TEST - 1;
  for ( ;rbeg!=rend;++rbeg, --i)
  {
    if ( i < 0 )
    {
      t << fatal( "FUCK" );
      break;
    }
    
    t << equal< expect, value_type > ( *rbeg, '0' + i%10 )
      << char(*rbeg)
      << "!=" << char('0' + i%10) << " [" << i << "] "
      << FAS_TESTING_FILE_LINE;
  }
      */

  

}

UNIT(test_unit, "")
{
  using namespace fas::testing;
  // typedef vset::memory::manager< /*fas::aspect< vset::allocator::value_type<char> >*/ vset::memory::fsb::aspect<char> > allocator_type;
  typedef vset::memory::manager< vset::memory::strategy::fsb_mmap<char> > allocator_type;

  std::cout << "test_unit {" << std::endl;
  
  allocator_type allocator;
  allocator.buffer().open("allocator.bin");
  std::cout << "-1-" << std::endl;
  allocator.buffer().truncate(0);
  std::cout << "-2-" << std::endl;
  test_char_init(t, allocator);
  std::cout << "-3-" << std::endl;
  test_char_test(t, allocator);
  std::cout << "-4-" << std::endl;
  allocator.buffer().sync();
  std::cout << "-5-" << std::endl;
  allocator.buffer().close();
  std::cout << "-6-" << std::endl;
  allocator.buffer().open("allocator.bin");
  std::cout << "-7-" << std::endl;
  test_char_test(t, allocator);
  std::cout << "-8-" << std::endl;
  allocator_type allocator2;
  std::cout << "-9-" << std::endl;
  allocator2.buffer().open("allocator.bin");
  std::cout << "-10-" << std::endl;
  test_char_test(t, allocator);
  std::cout << "-11-" << std::endl;
  allocator2.buffer().close();
  allocator.buffer().close();
  t << nothing();
  std::cout << "}test_unit " << std::endl;
}

UNIT(test_allocator, "")
{
  using namespace fas::testing;
  using namespace vset;

  std::cout << "test_allocator {" << std::endl;

  // typedef memory::manager< memory::fsb::aspect<value_type> > manager;
  typedef memory::manager< vset::memory::strategy::fsb_mmap<char> > manager;
  typedef memory::provider< manager > provider;
  typedef memory::allocator< memory::provider< manager > > allocator;

  manager m;
  allocator a(&m);
  a.memory().buffer().open("allocator2.bin");
//#warning не стираются
  a.memory().buffer().clear();
  *(a.allocate(1))='X';
  a.memory().buffer().sync();
  a.memory().buffer().close();
  t << nothing();
  std::cout << "}test_allocator " << std::endl;
}

BEGIN_SUITE(basic_suite, "")
  ADD_UNIT(test_unit)
  ADD_UNIT(test_allocator)
END_SUITE(basic_suite)
