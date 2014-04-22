
#include <vset/comparators/compare_member.hpp>
#include <vset/comparators/compare_list.hpp>
#include <vset/comparators/offset_compare.hpp>
#include <vset/comparators/pointer_compare.hpp>

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

int main()
{
  std::vector<data*> data_storage;

  typedef std::set< data*, vset::pointer_compare<compare1> > data1_type;
  typedef std::set< data*, vset::pointer_compare<compare2> > data2_type;
  data1_type data1;
  data2_type data2;

  data_storage.push_back(new data(1,2,3));
  data1.insert(data_storage.back());
  data2.insert(data_storage.back());

  data_storage.push_back(new data(1,3,4));
  data1.insert(data_storage.back());
  data2.insert(data_storage.back());

  data_storage.push_back(new data(1,3,5));
  data1.insert(data_storage.back());
  data2.insert(data_storage.back());

  for( data1_type::iterator itr = data1.begin(); itr != data1.end(); ++itr)
  {
    std::cout << (*itr)->data1 << " " << (*itr)->data2 << " " << (*itr)->data3 << std::endl;
  }

  std::cout << "---" << std::endl;
  
  for( data2_type::iterator itr = data2.begin(); itr != data2.end(); ++itr)
  {
    std::cout << (*itr)->data1 << " " << (*itr)->data2 << " " << (*itr)->data3 << std::endl;
  }

  for ( std::vector<data*>::iterator itr = data_storage.begin(); itr != data_storage.end(); ++itr)
  {
    delete (*itr);
  }
  
  return 0;
}
