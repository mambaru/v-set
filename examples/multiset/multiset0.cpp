#include <vset/multiset.hpp>
#include <vset/allocators/mmap_allocator.hpp>
#include <vset/allocators/filesync_allocator.hpp>
#include <iostream>


template<typename Tg>
struct stub1
{
  typedef fas::metalist::advice metatype;
  typedef Tg tag;
  typedef stub1<Tg> advice_class;

  advice_class& get_advice() { return *this;}
  const advice_class& get_advice() const { return *this;}

  template<typename T>
  typename T::iterator operator()(T&) const { return typename T::iterator(); }

  template<typename T, typename P1>
  typename T::iterator operator()(T&, P1) const { return typename T::iterator(); }

};

template<typename Tg>
struct stub2
{
  typedef fas::metalist::advice metatype;
  typedef Tg tag;
  typedef stub2<Tg> advice_class;

  advice_class& get_advice() { return *this;}
  const advice_class& get_advice() const { return *this;}

  template<typename T, typename Itr>
  Itr operator()(T& , Itr itr ) const { return itr;}
};

template<typename Tg>
struct stub3
{
  typedef fas::metalist::advice metatype;
  typedef Tg tag;
  typedef stub3<Tg> advice_class;

  advice_class& get_advice() { return *this;}
  const advice_class& get_advice() const { return *this;}

  template<typename T>
  typename T::container_type::iterator
  operator()(T&, const typename T::value_type& )
  {
    return typename T::container_type::iterator();
  }

  template<typename T>
  struct helper
  {
    typedef typename T::container_type container_type;
    typedef typename container_type::iterator iterator;
  };

  template<typename T>
  typename helper<T>::iterator
  operator()(T&, typename helper<T>::iterator , const typename T::key_type& )
  {
    return typename helper<T>::iterator();
  }

};




typedef fas::aspect< fas::type_list_n<
  stub1<vset::vtree::_insert_value_>,
  stub3<vset::vtree::_create_node_>,
  stub2<vset::vtree::_first_proper_node_>,
  stub3<vset::vtree::_split_node_>,
  stub2<vset::vtree::_update_node_key_>
>::type > stub;

int main()
{
  vset::multiset<int, std::less<int>, vset::filesync_allocator<3, vset::fsb_offset, stub> > vs;
  vs.insert(1);
  vs.insert(2);
  vs.erase(2);
  return 0;
}
