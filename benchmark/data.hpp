struct data
{
  int data1;
  int data2;
  int data3;
  int data4;
  int data5;
  int data6;
};

inline bool operator == (const data& l, const data& r)
{
  return l.data1==r.data1 && l.data2==r.data2 && l.data3==r.data3
        && l.data4==r.data4 && l.data5==r.data5 && l.data6==r.data6;
}
