
#include <fas/xtime.hpp>
#include <fas/typemanip.hpp>
#include <fas/type_list.hpp>
#include <set>
#include <iostream>

struct less
{
  template<typename V>
  bool operator()(const V& left, const V& right) const
  {
    return left < right;
  }
};

template<typename Ext, typename Comp = less>
struct key
{
  template<typename Obj>
  bool operator () (const Obj& left, const Obj& right) const
  {
    return _comp(_ext(left), _ext(right));
  }
private:
  Ext  _ext;
  Comp _comp;
};
/*
    return ( poller_id < right.poller_id )
           || ( !(right.poller_id < poller_id)
           && (content_id < right.content_id) );
           */


template<typename L = fas::empty_list>
struct composite_key
  : composite_key< typename fas::tail<L>::type >
{
  typedef composite_key< typename fas::tail<L>::type > super;

  template<typename Obj>
  bool operator () (const Obj& left, const Obj& right) const
  {
    return _key(left, right)
           || ( !_key(right, left )
           && super::operator()(left, right) );
  }
private:
  typename fas::head<L>::type _key;
};

template<>
struct composite_key<fas::empty_list>
{
  template<typename Obj>
  bool operator () (const Obj& , const Obj& ) const
  {
    return true;
  }
};

template<typename Ptr, typename C>
struct index_compare
{
  index_compare()
    : _left()
    , _right()
  {}

  index_compare(Ptr ptr)
    : _left(ptr)
    , _right(ptr)
  {}

  template<typename Ptr2>
  bool operator ()(Ptr2 left, Ptr2 right) const
  {
    _left = left;
    _right = right;
    return _comp(*_left, *_right);
  }

private:

  mutable Ptr _left;
  mutable Ptr _right;
  C _comp;
};

struct hit
{
  int src;
  int dst;
  int ts;

  hit(int src, int dst, int ts): src(src), dst(dst), ts(ts) {}
};

typedef index_compare<
  hit*,
  composite_key< fas::type_list_n<
    key< fas::member<hit, int, &hit::dst>, std::greater<int> >,
    key< fas::member<hit, int, &hit::src> >
  >::type >
> ptr_compare;

int main()
{
  std::set<hit*, ptr_compare> by_dst;
  by_dst.insert(new hit(1,1,1));
  by_dst.insert(new hit(2,2,2));
  by_dst.insert(new hit(3,3,3));
  by_dst.insert(new hit(2,3,3));
  by_dst.insert(new hit(1,3,3));

  for (auto i: by_dst)
    std::cout << i->src << " " << i->dst << std::endl;
  std::cin.get();
  return 0;
}