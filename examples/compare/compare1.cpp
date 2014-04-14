
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
  std::set< data*, vset::pointer_compare<compare1> > data1;
  std::set< data*, vset::pointer_compare<compare2> > data2;

  data_storage.push_back(new data{1,2,3});
  data1.insert(data_storage.back());
  data2.insert(data_storage.back());

  data_storage.push_back(new data{1,3,4});
  data1.insert(data_storage.back());
  data2.insert(data_storage.back());

  data_storage.push_back(new data{1,3,5});
  data1.insert(data_storage.back());
  data2.insert(data_storage.back());

  for (const data* d : data1)
  {
    std::cout << d->data1 << " " << d->data2 << " " << d->data3 << std::endl;
  }

  std::cout << "---" << std::endl;
  
  for (const data* d : data2)
  {
    std::cout << d->data1 << " " << d->data2 << " " << d->data3 << std::endl;
  }

  for (const data* d : data_storage)
  {
    delete d;
  }
  
  return 0;
}
