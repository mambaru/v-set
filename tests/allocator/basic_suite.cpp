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
#include <vset/buffer/persistent/filesync/aspect_filesync.hpp>

#define MAX_TEST 6400

template<typename T, typename Alloc>
void test_char_init(T& /*t*/, Alloc& allocator)
{
  using namespace fas::testing;
  typedef typename Alloc::pointer pointer;

  for (int i=0; i < MAX_TEST; ++i)
  {
    char* ch = allocator.allocate(1);
    if (!ch) throw;
    *ch = '0' + i%10;
  }

  pointer ptr = allocator.begin();
  //ptr[19]='X';
  //*ptr = '1';

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
  {
    t << equal< assert, char > ( *beg, '0' + i%10 ) << char(*beg) << "!=" << char('0' + i%10) << " " << FAS_TESTING_FILE_LINE;
  }

  t << equal< assert, int > ( i, MAX_TEST) << i << "!=" << MAX_TEST << " " << FAS_TESTING_FILE_LINE;
  
}

UNIT(test_unit, "")
{
  using namespace fas::testing;
  typedef vset::memory::manager< /*fas::aspect< vset::allocator::value_type<char> >*/ vset::memory::fsb::aspect<char> > allocator_type;
  
  
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
  t << nothing();
}

UNIT(test_allocator, "")
{
  using namespace fas::testing;
  using namespace vset;

  typedef memory::manager< memory::fsb::aspect<char> > manager;
  typedef memory::provider< manager > provider;
  typedef memory::allocator< memory::provider< manager > > allocator;

  manager m;
  allocator a(&m);
  a.memory().buffer().open("allocator2.bin");
//#warning не стираются
  a.memory().buffer().clear();
  *(a.allocate(1))='X';
  a.memory().buffer().sync();
  t << nothing();
}

BEGIN_SUITE(basic_suite, "")
  ADD_UNIT(test_unit)
  ADD_UNIT(test_allocator)
END_SUITE(basic_suite)
