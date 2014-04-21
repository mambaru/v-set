
#include <vset/comparators/compare_member.hpp>
#include <vset/multiset.hpp>
#include <fas/type_list/type_list_n.hpp>
#include <iostream>

struct employee
{
  int company_id;
  std::string name;
};

typedef ::vset::compare_member< employee, decltype(employee::company_id), &employee::company_id, std::less<int> > employee_compare;
typedef ::vset::multiset< employee, employee_compare> employee_multiset;

int main()
{
  employee_multiset employees;

  employees.insert( { {1, "Jane"}, {2, "Bob"}, {1, "Dave"}, {1, "John"} } );
  
  std::cout << "Count of all employees is " << employees.size() << std::endl;
  std::cout << "Count of company 1 employees is " << employees.count( {1, "Tmp"} ) << std::endl;

  std::cout << std::endl << "Employees list:" << std::endl;
  for(auto &emp: employees)
  {
    std::cout << "Company id: " << emp.company_id << ". Emp name: " << emp.name << std::endl;
  }

  auto company_one_range = employees.equal_range({1, "Tmp"});
  std::cout << std::endl << "Company 1 employees:" << std::endl;
  for( auto itr = company_one_range.first; itr != company_one_range.second; itr++)
  {
    std::cout << "  " << itr->name << std::endl;
  }

  std::cout << std::endl << "Lets fire Dave." << std::endl;
  employee_multiset::iterator company_one = employees.find( {1, "Tmp"} );
  employee_multiset::iterator dave_item;
  for( auto itr = company_one_range.first; itr != company_one_range.second; itr++)
  {
    if( itr->name == "Dave" )
    {
      dave_item = itr;
    }
  }

  employees.erase(dave_item);

  company_one_range = employees.equal_range({1, "Tmp"});
  std::cout << "Count of all employees is " << employees.size() << std::endl;
  std::cout << "Count of company 1 employees is " << employees.count( {1, "Tmp"} ) << std::endl;
  std::cout << "Company 1 employees (without Dave):" << std::endl;
  for( auto itr = company_one_range.first; itr != company_one_range.second; itr++)
  {
    std::cout << "  " << itr->name << std::endl;
  }

  std::cout << std::endl << "Company one is bankrupt!" << std::endl;
  employees.erase( {1, "Tmp"} );

  std::cout << "Count of all employees is " << employees.size() << std::endl;
  std::cout << "Count of company 1 employees is " << employees.count( {1, "Tmp"} ) << std::endl;
  std::cout << "Employees list:" << std::endl;
  for(auto &emp: employees)
  {
    std::cout << "Company id: " << emp.company_id << ". Emp name: " << emp.name << std::endl;
  }

  return 0;
}
