//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <vset/buffer/buffer.hpp>
#include <vset/buffer/persistent_buffer.hpp>
#include <vset/buffer/persistent/filesync/aspect_filesync.hpp>
#include <vset/buffer/persistent/mmap/aspect_mmap.hpp>
#include <vset/buffer/simple/aspect_simple.hpp>
#include <fas/testing.hpp>


template<typename T, typename B>
void first_test(T& t, B& buff)
{
  using namespace fas::testing;
  
  t << equal<expect, size_t>( 0, buff.size() ) << " first test: " <<  FAS_TESTING_FILE_LINE;
  t << equal<expect, size_t>( 0, buff.capacity() ) << " first test: "<< FAS_TESTING_FILE_LINE;
  buff.resize(10);
  buff.reserve(20);
  t << equal<expect, size_t>( 10, buff.size() ) << buff.size() << " first test: "<< FAS_TESTING_FILE_LINE;
  t << equal<expect, size_t>( 20, buff.capacity() )<< buff.capacity() << " first test: "<< FAS_TESTING_FILE_LINE;
  buff.data()[0]='0';
  buff.data()[1]='1';
  t << equal<expect, char>( buff.data()[0], '0' )<< " first test: "<< FAS_TESTING_FILE_LINE;
  t << equal<expect, char>( buff.data()[1], '1' )<< " first test: "<< FAS_TESTING_FILE_LINE;
  buff.clear();
  t << equal<expect, size_t>( 0, buff.size() )<< " first test: "<< FAS_TESTING_FILE_LINE;
  t << equal<expect, size_t>( 20, buff.capacity() )<< buff.capacity() << " first test: "<< FAS_TESTING_FILE_LINE;
  buff.resize(11);
  buff.truncate(7);
  t << equal<expect, size_t>( 7, buff.size() )<< " first test: "<< FAS_TESTING_FILE_LINE;
  t << equal<expect, size_t>( 7, buff.capacity() )<< " first test: "<< FAS_TESTING_FILE_LINE;
  buff.reserve(14);
  for (int i = 0 ; i < 7; ++i)
    buff.data()[i]='0'+i;
  
}

template<typename T, typename B>
void second_test(T& t, B& buff, const std::string& text)
{
  using namespace fas::testing;
  t << equal<expect, size_t>( 7, buff.size() ) << "second test (" << text << ")" << FAS_TESTING_FILE_LINE;
  t << equal<expect, size_t>( 14, buff.capacity() ) << "second test (" << text << ")"<< FAS_TESTING_FILE_LINE;
  
  for (int i = 0 ; i < 7; ++i)
    t << equal<expect, char>( buff.data()[i], '0'+i ) << "second test (" << text << "[" << i << "]!=" << buff.data()[i] << ")"<< FAS_TESTING_FILE_LINE;
}

UNIT(test_unit, "")
{
  using namespace fas::testing;
  vset::buffer::buffer< vset::buffer::simple::aspect_simple> buff;
  first_test( t, buff);
  second_test( t, buff, "simple");
  t << nothing();
}

UNIT(persistent_unit, "")
{
  using namespace fas::testing;
  using namespace vset::buffer;
  persistent_buffer<> pbuf;
  pbuf.open("test.bin");
  pbuf.truncate(0);
  
  first_test( t, pbuf);
  pbuf.sync();
  second_test( t, pbuf, "after clear");
  pbuf.close();
  pbuf.open("test.bin");
  second_test( t, pbuf, "after close/open");
  pbuf.open("test.bin");
  second_test( t, pbuf, "after reopen");
  persistent_buffer<> pbuf2;
  pbuf2.open("test.bin");
  second_test( t, pbuf2, "open exist");
  pbuf.close();
  pbuf2.close();
  t << nothing();
}

UNIT(mmap_unit, "")
{
  using namespace fas::testing;
  using namespace vset::buffer;
  persistent_buffer<persistent::mmap::aspect_mmap> pbuf;
  pbuf.open("test_mmap.bin");
  pbuf.truncate(0);

  first_test( t, pbuf);
  pbuf.sync();
  second_test( t, pbuf, "after clear");
  pbuf.close();
  pbuf.open("test_mmap.bin");
  second_test( t, pbuf, "after close/open");
  pbuf.open("test_mmap.bin");
  second_test( t, pbuf, "after reopen");
  persistent_buffer<persistent::mmap::aspect_mmap> pbuf2;
  pbuf2.open("test_mmap.bin");
  second_test( t, pbuf2, "open exist");
  pbuf.close();
  pbuf2.close();
  t << nothing();
}


BEGIN_SUITE(basic_suite, "")
  ADD_UNIT(test_unit)
  ADD_UNIT(persistent_unit)
  ADD_UNIT(mmap_unit)
END_SUITE(basic_suite)
