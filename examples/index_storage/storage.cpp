
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

  employee()
    : company_id(0)
    , division_id(0)
    , employee_id(0)
  {
  }

  employee(int cmp_id, int div_id, int emp_id)
    : company_id(cmp_id)
    , division_id(div_id)
    , employee_id(emp_id)
  {
  }
};

struct employee_key
{
  int company_id;
  int division_id;
  size_t offset;

  employee_key()
    : company_id(0)
    , division_id(0)
    , offset(0)
  {
  }

  employee_key(int cmp_id, int div_id, size_t off)
    : company_id(cmp_id)
    , division_id(div_id)
    , offset(off)
  {
  }

  employee_key(const employee& emp, size_t off)
    : company_id(emp.company_id)
    , division_id(emp.division_id)
    , offset(off)
  {
  }
};

//persistent storage
typedef ::vset::memory::manager< ::vset::memory::strategy::fsb_mmap<employee> > employees_storage;

//employee comparator
typedef ::vset::compare_list< fas::type_list_n<
  ::vset::compare_member< employee_key, int, &employee_key::company_id, std::less<int> >,
  ::vset::compare_member< employee_key, int, &employee_key::division_id, std::less<int> >
>::type > employee_cmp;

//inmemory index
typedef ::vset::multiset< employee_key, employee_cmp, ::vset::allocator<1024> > employee_index;

void insert_employee(employees_storage &storage, employee_index& index, const employee& emp)
{
  employees_storage::pointer ptr = storage.allocate(1);
  *ptr = emp;
  index.insert(employee_key(emp, ptr.get_offset()));
}

int main()
{
  employees_storage storage;

  //open persistent buffer
  storage.buffer().open( "./index_storage.bin" );

  employee_index index;

  //filling index by persistent data
  for ( employees_storage::pointer it = storage.begin(); it != storage.end(); ++it )
  {
    index.insert( employee_key( *it, it.get_offset() ) );
  }

  //output persistent data
  std::cout << "Data loaded from disk:" << std::endl;
  for( employee_index::iterator itr = index.begin(); itr != index.end(); ++itr )
  {
    std::cout << "Company id: " << itr->company_id
              << ". Division id: " << itr->division_id
              << ". Storage offset: " << itr->offset << std::endl;
  }

  //cleaning storage and index
  employees_storage::pointer ptr = storage.begin();
  for( employee_index::iterator itr = index.begin(); itr != index.end(); ++itr )
  {
    ptr.set_offset(itr->offset);
    storage.deallocate(ptr, 1);
  }
  index.clear();

  std::cout << std::endl << "Index size after erase " << index.size() << std::endl;

  insert_employee(storage, index, employee(1, 1, 101));
  insert_employee(storage, index, employee(2, 3, 105));
  insert_employee(storage, index, employee(1, 1, 108));

  std::cout << std::endl << "Data after insert:" << std::endl;
  for( employee_index::iterator itr = index.begin(); itr != index.end(); ++itr )
  {
    std::cout << "Company id: " << itr->company_id
              << ". Division id: " << itr->division_id
              << ". Storage offset: " << itr->offset << std::endl;
  }
  
  employee_index::iterator emp_ptr = index.find( employee_key(2, 3, 0ULL) );

  if( emp_ptr != index.end() )
  {
    employees_storage::pointer ptr = storage.begin();
    ptr.set_offset(emp_ptr->offset);
    std::cout << "Found employee from company 1 and division 3 - id " << ptr->employee_id << std::endl;
  }
  else
  {
    std::cout << "Found employee from company 1 and division 3 - id not found" << std::endl;
  }

  storage.buffer().close();

  return 0;
}
