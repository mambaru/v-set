//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#include <fas/testing.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/aspect/aspect.hpp>

//#include <pmi/vset.hpp>
#include <stdexcept>
#include <sstream>
#include <set>
#include <list>
#include <stack>
#include <vector>

std::stack<std::string> test_stack;

void raise(const std::string& text, const std::string& file = __FILE__, int pos = __LINE__  )
{
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

    for (;count!=0; curr+= step, --count)
      vect.push_back( static_cast<int>(curr) );

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
    for (;count!=0; --count)
      vect.push_back( first + std::rand()%dist );

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
    return false;

  typename Container1::const_iterator cur1 = cnt1.cbegin();
  typename Container2::const_iterator  cur2 = cnt2.cbegin();

  /*std::cout << "equal size " << cnt1.size() << std::endl;
  std::cout << "equal size " << cnt2.size() << std::endl;*/
  for (size_t i=0; i < cnt1.size(); ++i, ++cur1, ++cur2)
  {
    //std::cout << "equal " << *cur1 << "=="<< *cur2 << std::endl;
    if ( *cur1 != *cur2 )
      return false;
  }
  return true;
}

template<int ArraySize>
class persist_container
{
public:

  typedef vset::vtree::vtree< vset::vtree::aspect<int, std::less<int>, ArraySize> > set_type;
  //typedef typename vset_helper<int, std::less<int>, alloc_type::persistent, ArraySize >::vset_type set_type;
  //typedef vset<int> set_type;

  /*typedef typename set_type::value_compare value_compare;
  typedef typename set_type::allocator_type allocator_type;
  typedef typename allocator_type::allocation_manager allocation_manager;
  typedef typename allocation_manager::buffer buffer_type;
  */

  persist_container(const std::string& filename, bool clear)
  {
    _vset = new set_type;
    _vset->get_allocator().manager().buffer().open(filename);
    /*_buffer = new buffer_type;
    _buffer->open(filename.c_str(), 16);
    if (clear)
      _buffer->clear();
    _manager = new allocation_manager(*_buffer);
    _vset = new set_type( std::less<int>(), *_manager );
    */
  }

  ~persist_container()
  {
    delete _vset;
    /*delete _manager;
    delete _buffer;*/
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
  /*buffer_type* _buffer;
  allocation_manager* _manager;*/
  set_type* _vset;
};

template<int ArraySize>
class non_persist_container
{
public:
  // typedef vset<int, std::less<int>, std::allocator< sorted_array<int, 1024, std::less<int> > > > set_type;

  //typedef typename vset_helper<int, std::less<int>, alloc_type::inmemmory, ArraySize >::vset_type set_type;
  // TODO: в отдельный тест-файл
  typedef vset::vtree::vtree< vset::vtree::aspect<int, std::less<int>, ArraySize> > set_type;
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
void test_insert1(T& t, const Container& cnt, const F& init, bool onlyCheck)
{
  std::stringstream ss;
  if ( onlyCheck ) test_stack.push("test_insert1 onlyCheck");
  else test_stack.push("test_insert1");
  std::vector<int> values;
  init(values);
  if ( !onlyCheck )
    cnt->insert( values.begin(), values.end() );
  if ( values.size() != cnt->size() )
  {
    std::cout << values.size() << std::endl;
    std::cout << cnt->size() << std::endl;
    raise("values1.size() != cnt->size()", __FILE__, __LINE__);
  }
  std::sort(values.begin(), values.end());
  if (!equal(*cnt, values))
  {
    typename Container::iterator itr = cnt->begin();
    for (size_t i = 0; i < values.size(); ++i, ++itr )
    {
      std::cout << "[" << values[i] << "=" << /**(cnt->begin() + i) << */"=" << *itr<< "],";
      if ( values[i]!= *itr/**(cnt->begin() + i)*/)
      {
        std::cout << "<<<";
        /*for (size_t j = 1; j < 10; ++j )
          std::cout << "[" << values[j+1] << "=" << *(cnt->begin() + j+1) << "],";
        i = values.size()-10;
        */
      }
    }

    std::cout << std::endl;

    //cnt->check();
    std::cout << "END CHECK" << std::endl;
    raise("test_insert1", __FILE__, __LINE__);

  }
  test_stack.pop();
}


template<typename Container, typename T>
void test_insert(T& t, bool testPersist)
{
  test_stack.push("test_insert");

  test_insert1(t, Container("test_insert.bin", true), init_sequence(1, 1, 1), false );
  if ( testPersist ) test_insert1(t, Container("test_insert.bin", false), init_sequence(1, 1, 1), true );

  test_insert1(t, Container("test_insert.bin", true), init_sequence(10, 1, 5), false );
  if ( testPersist ) test_insert1(t, Container("test_insert.bin", false), init_sequence(10, 1, 5), true );

  test_insert1(t, Container("test_insert.bin", true), init_sequence(256, 1, 5), false );
  if ( testPersist ) test_insert1(t, Container("test_insert.bin", false), init_sequence(256, 1, 5), true );

  test_insert1(t, Container("test_insert.bin", true), init_sequence(2048, 1, 500), false );
  if ( testPersist ) test_insert1(t, Container("test_insert.bin", false), init_sequence(2048, 1, 500), true );

  init_random rnd(100000, 1, 1000);
  std::cout << "------------1-----------" << std::endl;
  test_insert1(t, Container("test_insert1.bin", true), rnd, false );
  std::cout << "------------2-----------" << std::endl;
  if ( testPersist ) test_insert1(t, Container("test_insert1.bin", false), rnd, true );

  test_stack.pop();
}

template<typename T, typename Container, typename F>
void test_erase1(T& t, const Container& cnt, const F& init, bool onlyCheck)
{
  std::cout << "void test_erase1(const Container& cnt, const F& init, bool onlyCheck) " << init.count << std::endl;
  std::stringstream ss;
  if ( onlyCheck ) test_stack.push("test_insert1 onlyCheck");
  else test_stack.push("test_erase1");
  std::list<int> values1, values2;
  init(values1);

  if ( !onlyCheck )
    cnt->insert( values1.begin(), values1.end() );

  std::list<int>::iterator beg = values1.begin();
  std::list<int>::iterator end = values1.end();


  // Перемещаем каждый второй

  for ( ;beg!=end; ++beg)
  {
    values2.push_back(*beg);
    values1.erase(beg++);
  }


  if ( !onlyCheck )
  {
    for ( std::list<int>::iterator beg = values2.begin(); beg!=values2.end(); ++beg )
    {

      /*std::cout << *beg << " [";
      for (auto tmpi = cnt->begin(); tmpi != cnt->end(); ++tmpi )
        std::cout << *tmpi <<",";
      std::cout << "] -> " ;*/

      cnt->erase( *beg );

      /*std::cout << " [";
      for (auto tmpi = cnt->begin(); tmpi != cnt->end(); ++tmpi )
        std::cout << *tmpi <<",";
      std::cout << "]" << std::endl;
      */

    }
    // cnt->check();
  }

  if ( values1.size() != cnt->size() )
  {
    std::cout << values1.size() << std::endl;
    std::cout << values2.size() << std::endl;
    std::cout << cnt->size() << std::endl;
    raise("values1.size() != cnt->size()", __FILE__, __LINE__);
  }

  if (!equal(*cnt, values1))
  {
    typename Container::iterator itr = cnt->begin();
    beg = values1.begin();
    for (size_t i = 0; i < values1.size(); ++i, ++itr, ++beg )
    {
      std::cout << "[" << *beg << "=" << /**(cnt->begin() + i) << */"=" << *itr<< "],";
      if ( *beg != *itr /**(cnt->begin() + i)*/)
      {
        std::cout << "<<<";
        /*for (size_t j = 1; j < 10; ++j )
          std::cout << "[" << values[j+1] << "=" << *(cnt->begin() + j+1) << "],";
        i = values.size()-10;*/
      }
    }

    std::cout << std::endl;

    raise("test_erase1", __FILE__, __LINE__);
  }
  test_stack.pop();
}

template<typename Container, typename T>
void test_erase(T& t, bool testPersist)
{
  test_stack.push("test_erase");


  test_erase1( t, Container("test_erase.bin", true), init_sequence(1, 1, 1), false );
  if ( testPersist ) test_erase1(t, Container("test_erase.bin", false), init_sequence(1, 1, 1), true );


  test_erase1( t,  Container("test_erase.bin", true), init_sequence(10, 1, 20), false );

  if ( testPersist ) test_erase1(t, Container("test_erase.bin", false), init_sequence(10, 1, 20), true );

  test_erase1( t, Container("test_erase.bin", true), init_sequence(256, 1, 512), false );
  if ( testPersist ) test_erase1(t, Container("test_erase.bin", false), init_sequence(256, 1, 512), true );

  test_erase1( t, Container("test_erase.bin", true), init_sequence(2048, 0, 2048), false );
  if ( testPersist ) test_erase1(t, Container("test_erase.bin", false), init_sequence(2048, 0, 2048), true );

  init_sequence init(10000, 0, 100000);
  test_erase1( t, Container("test_erase.bin", true), init, false );
  if ( testPersist ) test_erase1(t, Container("test_erase.bin", false), init, true );


  test_stack.pop();
}


template<typename Container, typename T>
void test_all(T& t, bool testPersist)
{
  test_stack.push("test_all");
  test_insert<Container>(t, testPersist);
  test_erase<Container>(t, testPersist);
  test_stack.pop();
}

template<int BlockSize, typename T>
void test_persist(T& t)
{
  std::cout << "------------------- test_persist ------------------- " << BlockSize << std::endl;
  test_stack.push("test_persist");
  typedef persist_container<BlockSize> container;
  test_all<container>(t, true);
  test_stack.pop();
  // test_all(persist_container("set.bin", true), !clear);
}

template<int BlockSize, typename T>
void test_non_persist(T& t)
{
  std::cout << "------------------- test_non_persist ------------------- " << BlockSize << std::endl;
  test_stack.push("test_non_persist");
  typedef non_persist_container<BlockSize> container;
  test_all<container>(t, false);
  test_stack.pop();
}

template<typename T>
void test_all_persist(T& t)
{
  std::cout << "------------------- test_all_persist -------------------" << std::endl;
  test_stack.push("test_all_persist");

  test_persist<3>(t);
  test_persist<4>(t);
  test_persist<5>(t);
  test_persist<6>(t);
  test_persist<7>(t);
  test_persist<13>(t);
  test_persist<32>(t);
  test_persist<64>(t);
  test_persist<128>(t);
  test_persist<256>(t);
  test_persist<500>(t);
  test_persist<1000>(t);

  test_persist<1024>(t);
  test_persist<4000>(t);
  test_stack.pop();
}

template<typename T>
void test_all_non_persist(T& t)
{
  std::cout << "------------------- test_all_non_persist -------------------" << std::endl;
  test_stack.push("test_all_non_persist");

  test_non_persist<3>(t);
  test_non_persist<4>(t);
  test_non_persist<5>(t);
  test_non_persist<6>(t);
  test_non_persist<7>(t);
  test_non_persist<13>(t);
  test_non_persist<32>(t);
  test_non_persist<64>(t);
  test_non_persist<128>(t);
  test_non_persist<256>(t);
  test_non_persist<500>(t);
  test_non_persist<1000>(t);
  test_non_persist<1024>(t);
  test_non_persist<4000>(t);

  test_stack.pop();
}
UNIT(vtree_mmap_test, "")
{
  test_all_non_persist(t);
  test_all_persist(t);
}

BEGIN_SUITE(vtree_mmap_suite, "")
  ADD_UNIT(vtree_mmap_test)
END_SUITE(vtree_mmap_suite)
