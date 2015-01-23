
#include <vset/comparators/compare_member.hpp>
#include <vset/comparators/compare_list.hpp>
#include <vset/comparators/offset_compare.hpp>
#include <vset/comparators/pointer_compare.hpp>

#include <vset/memory/manager.hpp>
#include <vset/allocators/mmap_allocator.hpp>
#include <vset/allocators/allocator.hpp>
#include <vset/multiset.hpp>

#include <fas/type_list/type_list_n.hpp>
#include <iostream>

typedef unsigned short offset_t;

struct employee
{
  int company_id;
  int division_id;
  int employee_id;
};

//persistent storage
typedef ::vset::memory::manager< ::vset::memory::strategy::fsb_mmap<employee> > employees_storage;

//employee comparator
typedef ::vset::compare_list< fas::type_list_n<
  ::vset::compare_member< employee, int, &employee::company_id, std::less<int> >,
  ::vset::compare_member< employee, int, &employee::division_id, std::less<int> >
>::type > employee_cmp;

typedef ::vset::offset_compare<
  offset_t,
  employees_storage,
  employee_cmp
> employee_offset_cmp;

//inmemory index
typedef ::vset::multiset< offset_t, employee_offset_cmp, ::vset::mmap_allocator<1024> > employee_index;

int main()
{
  employees_storage storage;
  employees_storage::pointer temp;

  //open persistent buffer
  storage.buffer().open( "./employee_storage.bin" );

  employee_index index( employee_offset_cmp(storage.end()) );
  index.get_allocator().memory().buffer().open( "./employee_index.bin" );
  
  if ( storage.buffer().size() == 0 )
  {
    std::cout << "initialize..." << std::endl;
    temp = storage.allocate(1);
    for (int i = 0; i < 1000; ++i)
    {
      auto ptr = storage.allocate(1);
      *ptr = employee{i,i,i};
      index.insert( ptr.get_offset() );
    }
  }
  else
  {
    temp = storage.begin();
  }
  
  *temp = employee{500,500,0};
  auto itr = index.find(temp.get_offset() );
  if ( itr!=index.end() )
  {
    auto ptr = storage.end();
    ptr.set_offset(*itr);
    std::cout << "company_id=" << ptr->company_id << std::endl;
    std::cout << "division_id="<< ptr->division_id << std::endl;
    std::cout << "employee_id="<< ptr->employee_id << std::endl;
  }
  
  

  //filling index by persistent data
  //for ( employees_storage::pointer it = storage.begin(); it != storage.end(); ++it )
  //{
  //    index.insert( employee_key( *it, it.get_offset() ) );
  //}

  index.get_allocator().memory().buffer().close();
  storage.buffer().close();

  return 0;
}
