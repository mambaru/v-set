
#include <vset/comparators/compare_member.hpp>
#include <vset/comparators/compare_list.hpp>
#include <vset/comparators/offset_compare.hpp>
#include <vset/comparators/pointer_compare.hpp>

#include <vset/memory/fsb_mmap.hpp>
#include <vset/allocators/mmap_allocator.hpp>
#include <vset/multiset.hpp>

#include <fas/type_list/type_list_n.hpp>
#include <set>
#include <vector>
#include <iostream>

struct data
{
  int data1;
  int data2;
  int data3;

  data()
    : data1(),
      data2(),
      data3()
  {
  }

  data(int d1, int d2, int d3)
    : data1(d1),
      data2(d2),
      data3(d3)
  {
  }
};

struct compare1:
  vset::compare_list< fas::type_list_n<
    vset::compare_member< data, int, &data::data1, std::less<int> >,
    vset::compare_member< data, int, &data::data2, std::greater<int> >,
    vset::compare_member< data, int, &data::data3, std::less<int> >
  >::type >
{
};

struct compare2:
  vset::compare_list< fas::type_list_n<
    vset::compare_member< data, int, &data::data1, std::less<int> >,
    vset::compare_member< data, int, &data::data3, std::less<int> >,
    vset::compare_member< data, int, &data::data2, std::greater<int> >
  >::type >
{
};

typedef unsigned int offset_t;

struct data_storage: vset::memory::fsb_mmap<data> {};

typedef vset::offset_compare< offset_t, data_storage, compare1> offset_compare1_type;
struct offset_compare1:
  offset_compare1_type
{
  offset_compare1():offset_compare1_type(){}
  explicit offset_compare1(data_storage::pointer ptr):offset_compare1_type(ptr){}
};

typedef vset::offset_compare< offset_t, data_storage, compare2> offset_compare2_type;
struct offset_compare2:
  offset_compare2_type
{
  offset_compare2():offset_compare2_type(){}
  explicit offset_compare2(data_storage::pointer ptr):offset_compare2_type(ptr){}
};

typedef vset::multiset<offset_t, offset_compare1, vset::mmap_allocator<512> > index1;
typedef vset::multiset<offset_t, offset_compare2, vset::mmap_allocator<512> > index2;

int main()
{
  data_storage stg;
  index1 idx1( offset_compare1(stg.end() ) );
  index2 idx2( offset_compare2(stg.end() ) );
  stg.buffer().open("storage.bin");
  idx1.get_allocator().memory().buffer().open("index1.bin");
  idx2.get_allocator().memory().buffer().open("index2.bin");

  stg.buffer().clear();
  idx1.clear();
  idx2.clear();
  
  data_storage::pointer ptr = stg.allocate(1);
  *ptr = data(1,2,3);
  idx1.insert( ptr.get_offset() );
  idx2.insert( ptr.get_offset() );

  ptr = stg.allocate(1);
  *ptr = data(1,3,4);
  idx1.insert( ptr.get_offset() );
  idx2.insert( ptr.get_offset() );
  
  ptr = stg.allocate(1);
  *ptr = data(1,3,5);
  idx1.insert( ptr.get_offset() );
  idx2.insert( ptr.get_offset() );

  ptr = stg.end();
  
  for( index1::iterator itr = idx1.begin(); itr != idx1.end(); ++itr )
  {
    ptr.set_offset(*itr);
    std::cout << ptr->data1 << " " << ptr->data2 << " " << ptr->data3 << std::endl;
  }

  std::cout << "---" << std::endl;

  for( index2::iterator itr = idx2.begin(); itr != idx2.end(); ++itr )
  {
    ptr.set_offset(*itr);
    std::cout << ptr->data1 << " " << ptr->data2 << " " << ptr->data3 << std::endl;
  }
  return 0;
}
