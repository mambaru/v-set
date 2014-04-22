
#include <vset/comparators/compare_member.hpp>
#include <vset/multiset.hpp>
#include <fas/type_list/type_list_n.hpp>
#include <iostream>

struct employee
{
  int company_id;
  std::string name;

  employee()
    : company_id()
    , name()
  {
  }

  employee(int cmp_id, std::string nm)
    : company_id(cmp_id)
    , name(nm)
  {

  }
};

typedef ::vset::compare_member< employee, int, &employee::company_id, std::less<int> > employee_compare;
typedef ::vset::multiset< employee, employee_compare> employee_multiset;

int main()
{
  employee_multiset employees;

  employees.insert( employee(1, "Jane") );
  employees.insert( employee(2, "Bob") );
  employees.insert( employee(1, "Dave") );
  employees.insert( employee(1, "John") );
  
  std::cout << "Count of all employees is " << employees.size() << std::endl;
  std::cout << "Count of company 1 employees is " << employees.count( employee(1, "Tmp") ) << std::endl;

  std::cout << std::endl << "Employees list:" << std::endl;
  for( employee_multiset::iterator itr = employees.begin(); itr != employees.end(); ++itr)
  {
    std::cout << "Company id: " << itr->company_id << ". Emp name: " << itr->name << std::endl;
  }

  std::pair<employee_multiset::iterator, employee_multiset::iterator> company_one_range = employees.equal_range( employee(1, "Tmp") );
  std::cout << std::endl << "Company 1 employees:" << std::endl;
  for( employee_multiset::iterator itr = company_one_range.first; itr != company_one_range.second; itr++)
  {
    std::cout << "  " << itr->name << std::endl;
  }

  std::cout << std::endl << "Lets fire Dave." << std::endl;;
  employee_multiset::iterator dave_item;
  for( employee_multiset::iterator itr = company_one_range.first; itr != company_one_range.second; itr++)
  {
    if( itr->name == "Dave" )
    {
      dave_item = itr;
    }
  }

  employees.erase(dave_item);

  company_one_range = employees.equal_range( employee(1, "Tmp") );
  std::cout << "Count of all employees is " << employees.size() << std::endl;
  std::cout << "Count of company 1 employees is " << employees.count( employee(1, "Tmp") ) << std::endl;
  std::cout << "Company 1 employees (without Dave):" << std::endl;
  for( employee_multiset::iterator itr = company_one_range.first; itr != company_one_range.second; itr++)
  {
    std::cout << "  " << itr->name << std::endl;
  }

  std::cout << std::endl << "Company one is bankrupt!" << std::endl;
  employees.erase( employee(1, "Tmp") );

  std::cout << "Count of all employees is " << employees.size() << std::endl;
  std::cout << "Count of company 1 employees is " << employees.count( employee(1, "Tmp") ) << std::endl;
  std::cout << "Employees list:" << std::endl;
  for( employee_multiset::iterator itr = employees.begin(); itr != employees.end(); ++itr)
  {
    std::cout << "Company id: " << itr->company_id << ". Emp name: " << itr->name << std::endl;
  }

  return 0;
}
