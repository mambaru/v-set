#include "data.hpp"

extern "C"
{

bool cmp3fe(const data& l, const data& r)
{
  return   l.data1 < r.data1 ? true : l.data1 > r.data1 ? false
           : l.data2 < r.data2 ? true : l.data2 > r.data2 ? false 
           : l.data3 > r.data3 ? true : l.data3 < r.data3 ? false 
           : l.data4 < r.data4 ? true : l.data4 > r.data4 ? false 
           : l.data5 < r.data5 ? true : l.data5 > r.data5 ? false 
           : l.data6 < r.data6 ? true : false;
}

}
