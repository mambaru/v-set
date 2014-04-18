//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>

#include <stdexcept>
#include <sstream>
#include <set>
#include <list>
#include <stack>
#include <vector>

std::stack<std::string> test_stack;
std::stringstream output_buffer;

void raise(const std::string& text, const std::string& file = __FILE__, int pos = __LINE__  )
{
  std::cout << output_buffer.str();
  while ( !test_stack.empty() )
  {
    std::cout << test_stack.top() << std::endl;
    test_stack.pop();
  }
  std::stringstream ss;
  ss << file << ":" << pos << " " << std::endl << text;
  throw std::logic_error(ss.str());
}

struct init_sequence
{
  int count;
  int first;
  int last;

  std::vector<int> vect;

  init_sequence(int count, int first, int last)
    : count(count)
    , first(first)
    , last(last)
  {
    double step = double(last - first)/count;
    double curr = first;

    for (; count != 0; curr += step, --count)
    {
      vect.push_back( static_cast<int>(curr) );
    }
  }

  template<typename Container>
  void operator()(Container& cnt) const
  {
    std::copy(vect.begin(), vect.end(), std::back_inserter(cnt));
  }
};

struct init_random
{
  int count;
  int first;
  int last;

  std::vector<int> vect;

  init_random(int count, int first, int last)
    : count(count)
    , first(first)
    , last(last)
  {
    int dist = last - first;
    for (; count != 0; --count)
    {
      vect.push_back( first + std::rand()%dist );
    }
  }

  template<typename Container>
  void operator()(Container& cnt)  const
  {
    std::copy(vect.begin(), vect.end(), std::back_inserter(cnt));
  }
};


template<typename Container1, typename Container2>
bool equal( const Container1& cnt1, const Container2& cnt2)
{
  if (cnt1.size() != cnt2.size() )
  {
    return false;
  }

  typename Container1::const_iterator cur1 = cnt1.begin();
  typename Container2::const_iterator  cur2 = cnt2.begin();
  for (size_t i=0; i < cnt1.size(); ++i, ++cur1, ++cur2)
  {
    if ( *cur1 != *cur2 )
    {
      return false;
    }
  }
  return true;
}

template<int ArraySize>
class persist_container
{
public:

  typedef vset::vtree::vtree< vset::vtree::strategy::vtree_fsb_mmap<int, std::less<int>, ArraySize> > set_type;
  typedef typename set_type::iterator iterator;
  typedef typename set_type::const_iterator const_iterator;

  persist_container(const std::string& filename, bool clear)
  {
    output_buffer << "persist_container::persist_container() clear = " << clear << std::endl;
    
    _vset = new set_type;
    
    _vset->get_allocator().memory().buffer().open( filename.c_str() );
    _vset->get_allocator().memory().buffer().reserve( 1024*1024);
    output_buffer << "persist_container size after open: " << _vset->size() << std::endl;
    if ( clear )
    {
      _vset->clear();
      output_buffer << "persist_container size after clear: " << _vset->size() << std::endl;
    }
  }

  ~persist_container()
  {
    output_buffer << "~persist_container() " << _vset->size() << std::endl;
    output_buffer << "~persist_container() " << _vset->get_allocator().memory().buffer().size() << std::endl;

    typedef typename set_type::allocator_type::memory_type::pointer pointer;
    pointer beg = _vset->get_allocator().memory().begin();
    pointer end = _vset->get_allocator().memory().end();
    size_t count = 0;
    for (;beg != end; ++beg)
    {
      count += beg->size();
    }
    output_buffer << "~persist_container() " << count << std::endl;
    
    _vset->get_allocator().memory().buffer().sync();
    _vset->get_allocator().memory().buffer().close();
    delete _vset;
  }

  set_type& operator*() const
  {
    return *_vset;
  }

  set_type* operator->() const
  {
    return _vset;
  }

private:
  persist_container(const persist_container& ) = delete;
  set_type* _vset;
};

template<int ArraySize>
class non_persist_container
{
public:

  typedef vset::vtree::vtree< vset::vtree::strategy::vtree_fsb_mmap<int, std::less<int>, ArraySize> > set_type;
  typedef typename set_type::iterator iterator;

  non_persist_container(const std::string& filename, bool /*clear*/)
  {
    _vset = new set_type;
    _vset->get_allocator().memory().buffer().open(filename.c_str());
  }

  ~non_persist_container()
  {
    delete _vset;
  }

  set_type& operator*() const
  {
    return *_vset;
  }

  set_type* operator->() const
  {
    return _vset;
  }

private:
  set_type* _vset;
};


template<typename T, typename Container, typename F>
void test_insert1(T& /*t*/, const Container& cnt, const F& init, bool onlyCheck)
{
  output_buffer << "test_insert1 onlyCheck=" << onlyCheck << "{"  << std::endl;
  std::stringstream ss;
  if ( onlyCheck )
  {
    test_stack.push("test_insert1 onlyCheck");
  }
  else
  {
    test_stack.push("test_insert1");
  }
  std::vector<int> values;
  init(values);
  if ( !onlyCheck )
  {
    cnt->insert( values.begin(), values.end() );
  }
  output_buffer << "<<<--- 1" << std::endl;
  if ( values.size() != cnt->size() )
  {
    output_buffer << "----------------------" << std::endl;
    output_buffer << values.size() << std::endl;
    output_buffer << cnt->size() << std::endl;
    output_buffer << "======================" << std::endl;
    raise("values1.size() != cnt->size()", __FILE__, __LINE__);
  }
  output_buffer << "<<<--- 2" << std::endl;
  std::sort(values.begin(), values.end());
  output_buffer << "<<<--- 3" << std::endl;
  if (!equal(*cnt, values))
  {
    typename Container::iterator itr = cnt->begin();
    for (size_t i = 0; i < values.size(); ++i, ++itr )
    {
      output_buffer << "<<<--- X" << std::endl;
      output_buffer << "[" << values[i] << "=" << "=" << *itr<< "],";
      if ( values[i] != *itr )
      {
        output_buffer << "<<<";
      }
    }

    output_buffer << std::endl << "END CHECK" << std::endl;
    raise("test_insert1", __FILE__, __LINE__);

  }
  test_stack.pop();
  output_buffer << "}test_insert1"<< std::endl;
}


template<typename Container, typename T>
void test_insert(T& t)
{
  output_buffer << "test_insert {" << std::endl;
  
  test_stack.push("test_insert");

  test_insert1(t, Container("test_insert.bin", true), init_sequence(1, 1, 1), false );
  test_insert1(t, Container("test_insert.bin", false), init_sequence(1, 1, 1), true );

  test_insert1(t, Container("test_insert.bin", true), init_sequence(10, 1, 5), false );
  test_insert1(t, Container("test_insert.bin", false), init_sequence(10, 1, 5), true );

  test_insert1(t, Container("test_insert.bin", true), init_sequence(256, 1, 5), false );
  test_insert1(t, Container("test_insert.bin", false), init_sequence(256, 1, 5), true );

  test_insert1(t, Container("test_insert.bin", true), init_sequence(2048, 1, 500), false );
  test_insert1(t, Container("test_insert.bin", false), init_sequence(2048, 1, 500), true );

  init_random rnd(100000, 1, 1000);
  output_buffer << "------------1-----------" << std::endl;
  test_insert1(t, Container("test_insert1.bin", true), rnd, false );
  output_buffer << "------------2-----------" << std::endl;
  test_insert1(t, Container("test_insert1.bin", false), rnd, true );

  test_stack.pop();
  output_buffer << "}test_insert" << std::endl;
}

template<typename T, typename Container, typename F>
void test_erase1(T& /*t*/, const Container& cnt, const F& init, bool onlyCheck)
{
  output_buffer << "void test_erase1(const Container& cnt, const F& init, bool onlyCheck) " << init.count << std::endl;
  std::stringstream ss;
  if ( onlyCheck )
  {
    test_stack.push("test_insert1 onlyCheck");
  }
  else
  {
    test_stack.push("test_erase1");
  }
  std::list<int> values1, values2;
  init(values1);

  if ( !onlyCheck )
  {
    cnt->insert( values1.begin(), values1.end() );
  }

  std::list<int>::iterator beg = values1.begin();
  std::list<int>::iterator end = values1.end();

  // Перемещаем каждый второй
  for ( ;beg!=end; ++beg)
  {
    values2.push_back(*beg);
    try
    {
      values1.erase(beg++);
    }
    catch(const std::exception& e)
    {
      raise( e.what(), __FILE__, __LINE__);
    }
  }

  if ( !onlyCheck )
  {
    for ( std::list<int>::iterator beg = values2.begin(); beg!=values2.end(); ++beg )
    {
      try
      {
        if ( cnt->find( *beg ) == cnt->end() )
        {
          output_buffer << "fatal " << *beg << std::endl ;
          raise( "Cannot find value while iterating", __FILE__, __LINE__);
        }

        typename Container::iterator upper = cnt->upper_bound(*beg);
        if ( upper != cnt->end() )
        {
          if ( *upper == *beg )
          {
            output_buffer << "fatal2" << *upper << std::endl ;
          }
          else
          {
            output_buffer << "UPPER " << *upper << std::endl ;
          }
        }
        cnt->erase( *beg );
      }
      catch(const std::exception& e)
      {
        raise( e.what(), __FILE__, __LINE__);
      }
    }
  }

  if ( values1.size() != cnt->size() )
  {
    output_buffer << values1.size() << std::endl;
    output_buffer << values2.size() << std::endl;
    output_buffer << cnt->size() << std::endl;
    raise("values1.size() != cnt->size()", __FILE__, __LINE__);
  }

  if (!equal(*cnt, values1))
  {
    typename Container::iterator itr = cnt->begin();
    beg = values1.begin();
    for (size_t i = 0; i < values1.size(); ++i, ++itr, ++beg )
    {
      output_buffer << "[" << *beg << "=" << "=" << *itr<< "],";
      if ( *beg != *itr )
      {
        output_buffer << "<<<";
      }
    }

    output_buffer << std::endl;

    raise("test_erase1", __FILE__, __LINE__);
  }
  test_stack.pop();
}

template<typename Container, typename T>
void test_erase(T& t)
{
  test_stack.push("test_erase");

  test_erase1( t, Container("test_erase.bin", true), init_sequence(1, 1, 1), false );
  test_erase1(t, Container("test_erase.bin", false), init_sequence(1, 1, 1), true );

  test_erase1( t,  Container("test_erase.bin", true), init_sequence(10, 1, 20), false );
  test_erase1(t, Container("test_erase.bin", false), init_sequence(10, 1, 20), true );

  test_erase1( t, Container("test_erase.bin", true), init_sequence(256, 1, 512), false );
  test_erase1(t, Container("test_erase.bin", false), init_sequence(256, 1, 512), true );

  test_erase1( t, Container("test_erase.bin", true), init_sequence(2048, 0, 2048), false );
  test_erase1(t, Container("test_erase.bin", false), init_sequence(2048, 0, 2048), true );

  init_sequence init(10000, 0, 100000);
  test_erase1( t, Container("test_erase.bin", true), init, false );
  test_erase1(t, Container("test_erase.bin", false), init, true );

  test_stack.pop();
}


template<typename Container, typename T>
void test_all(T& t)
{
  test_stack.push("test_all");
  output_buffer << "test_all insert {" << std::endl;
  test_insert<Container>(t);
  output_buffer << "}test_all insert" << std::endl;
  output_buffer << "test_all erase {" << std::endl;
  test_erase<Container>(t);
  output_buffer << "}test_all erase" << std::endl;
  test_stack.pop();
}

template<int BlockSize, typename T>
void test_persist(T& t)
{
  ::truncate( "./test_erase.bin", 0);
  ::truncate( "./test_insert.bin", 0);
  ::truncate( "./test_insert1.bin", 0);
  output_buffer << "------------------- test_persist ------------------- " << BlockSize << std::endl;
  test_stack.push("test_persist");
  typedef persist_container<BlockSize> container;
  test_all<container>(t);
  test_stack.pop();
}

template<typename T>
void test_all_persist(T& t)
{
  output_buffer << "------------------- test_all_persist -------------------" << std::endl;
  test_stack.push("test_all_persist");

  test_persist<3>(t);
  test_persist<4>(t);
  test_persist<5>(t);
  test_persist<7>(t);
  /*test_persist<13>(t);
  test_persist<64>(t);
  test_persist<256>(t);
  test_persist<500>(t);
  test_persist<1000>(t);
  test_persist<1024>(t);
  test_persist<4000>(t);*/

  test_stack.pop();

  t << fas::testing::nothing();
}

UNIT(vtree_mmap_test, "")
{
  using namespace fas::testing;
  test_all_persist(t);
  t << nothing();
}

BEGIN_SUITE(vtree_mmap_suite, "")
  ADD_UNIT(vtree_mmap_test)
END_SUITE(vtree_mmap_suite)
