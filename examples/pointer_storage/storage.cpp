
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

struct employee
{
  int company_id;
  int division_id;
  int employee_id;
};

typedef size_t offset_t;

//persistent storage
typedef ::vset::memory::manager< ::vset::memory::strategy::fsb_mmap<employee> > employees_storage;

//employee comparator
typedef ::vset::compare_list< fas::type_list_n<
  ::vset::compare_member< employee, decltype(employee::company_id), &employee::company_id, std::less<decltype(employee::company_id)> >,
  ::vset::compare_member< employee, decltype(employee::division_id), &employee::division_id, std::less<decltype(employee::division_id)> >
>::type > employee_cmp;

typedef ::vset::offset_compare< offset_t, employees_storage, employee_cmp > employee_offset_cmp;

struct EmployeeCmp : employee_offset_cmp
{
  EmployeeCmp( ){}
  EmployeeCmp( employees_storage::pointer ptr ): employee_offset_cmp( ptr ){}
};

//inmemory index
typedef ::vset::multiset< EmployeeCmp::offset_t, EmployeeCmp, ::vset::allocator<1024> > EmployeeIndex;

int main()
{
  employees_storage storage;

  //open persistent buffer
  storage.buffer().open( "./pointer_storage.bin" );

  EmployeeIndex index( EmployeeCmp( storage.end() ) );

  //initialize service pointer
  employees_storage::pointer worker_pointer;
  if( storage.begin() == storage.end() )
  {
    worker_pointer = storage.allocate(1);
  }
  else
  {
    worker_pointer = storage.begin();
  }
  new (worker_pointer) employee();

  //filling index by persistent data
  for ( employees_storage::pointer it = (storage.begin() + 1); it != storage.end(); ++it )
  {
    index.insert( it.get_offset() );
  }

  //output persistent data
  std::cout << "Data loaded from disk:" << std::endl;
  for(auto emp: index)
  {
    worker_pointer.set_offset(emp);
    std::cout << "Company id: " << worker_pointer->company_id
              << ". Division id: " << worker_pointer->division_id
              << ". Emp id: " << worker_pointer->employee_id << std::endl;
  }

  //cleaning storage and index
  for(auto emp: index)
  {
    worker_pointer.set_offset(emp);
    storage.deallocate(worker_pointer, 1);
  }
  worker_pointer = storage.begin();
  index.clear();

  std::cout << std::endl << "Index size after erase " << index.size() << std::endl;

  employee worker;

  worker = {1, 1, 101};
  offset_t worker_offset = storage.allocate(1).get_offset();
  worker_pointer.set_offset(worker_offset);
  new (worker_pointer) employee();
  *worker_pointer = worker;
  index.insert(worker_offset);

  worker = {2, 3, 105};
  worker_offset = storage.allocate(1).get_offset();
  worker_pointer.set_offset(worker_offset);
  new (worker_pointer) employee();
  *worker_pointer = worker;
  index.insert(worker_offset);

  worker = {1, 1, 108};
  worker_offset = storage.allocate(1).get_offset();
  worker_pointer.set_offset(worker_offset);
  new (worker_pointer) employee();
  *worker_pointer = worker;
  index.insert(worker_offset);

  std::cout << std::endl << "Data after insert:" << std::endl;
  for(auto emp: index)
  {
    worker_pointer.set_offset(emp);
    std::cout << "Company id: " << worker_pointer->company_id
              << ". Division id: " << worker_pointer->division_id
              << ". Emp id: " << worker_pointer->employee_id << std::endl;
  }
  
  worker_pointer = storage.begin();
  *worker_pointer = {2,3,0};
  auto emp_ptr = index.find( worker_pointer.get_offset() );
  worker_pointer.set_offset(*emp_ptr);
  std::cout << "Found employee from company 1 and division 3 - id " << worker_pointer->employee_id << std::endl;

  storage.buffer().close();

  return 0;
}
