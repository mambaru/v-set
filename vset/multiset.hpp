#ifndef VSET_MULTI_SET_HPP
#define VSET_MULTI_SET_HPP

#include <fas/typemanip/has_typename.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>

namespace vset{ 

template<typename V, typename C = std::less<V>, typename A = std::allocator<V> >
class multiset;
  
namespace aspect_maker
{
  FAS_HAS_TYPENAME(is_aspect_maker, aspect_maker)
  
  template<typename V, typename C, typename A>
  struct make
  {
    typedef typename A::template apply<V, C>::type aspect;
  };

  template<typename V, typename C, typename A, bool >
  struct multiset_impl;

  template<typename V, typename C, typename A>
  struct multiset_impl<V, C, A, true>
  {
    typedef vtree::vtree< typename make<V, C, A>::aspect  > type;
  };

  template<typename V, typename C, typename A>
  struct multiset_impl<V, C, A, false>
  {
    typedef vtree::vtree< vtree::strategy::vtree_std_alloc<V, C, 512> > type;
    //typedef vtree::vtree< typename make<V, C, A>::aspect  > type;
  };

  
  /** 
  * @internal секрет
  */
  template<typename V, typename C, typename A>
  struct multiset
  {
    typedef typename multiset_impl<V, C, A, aspect_maker::is_aspect_maker<A>::value >::type type;
  };
}
/** @endinternal */

/**
   @brief vset::multiset это ассоциативный контейнер, который содержит упорядоченный набор объектов типа V (допускаются ключи с одинаковыми значениями).
          Сортировка производится с помощью функции сравнения ключей Compare. Операции поиска, вставки и удаления имеют отличную от `std::multiset` 
          логорифмичскую сложность и определяется стратегией хранения, которая внедряется через аллокатор.
   @tparam V - тип хранимых значений 
   @tparam C - функции сравнения ключей
   @tparam A - аллокатор
   @inherit 
   @details details
*/
template<typename V, typename C, typename A >
class multiset
  : public aspect_maker::multiset<V, C, A >::type
{
  typedef typename aspect_maker::multiset<V, C, A >::type super;
  
public:
  typedef typename super::key_type    key_type;
  typedef typename super::value_type  value_type;
  typedef typename super::key_compare    key_compare;
  typedef typename super::value_compare  value_compare;
  typedef typename super::allocator_type allocator_type;
  typedef typename super::reference reference;
  typedef typename super::const_reference const_reference;
  typedef typename super::pointer pointer;
  typedef typename super::const_pointer const_pointer;
  typedef typename super::iterator iterator;
  typedef typename super::const_iterator const_iterator;
  typedef typename super::reverse_iterator reverse_iterator;
  typedef typename super::const_reverse_iterator const_reverse_iterator;
  typedef typename super::difference_type difference_type;
  typedef typename super::size_type size_type;
  
  /**
   * @brief Конструктор по умолчанию. Создаёт пустой контейнер.
   */
  multiset(): super() {}

  /**
   * @brief Конструктор создаёт пустой контейнер. 
   * @param comp функции сравнения ключей
   * @param alloc функции сравнения ключей
   */
  explicit multiset(const key_compare& comp, const allocator_type& alloc = allocator_type() )
    : super(comp, alloc)
  {
  }

  /**
   * @brief Создаёт контейнер с содержимым из диапазона [first, last).
   * @param first итератор начала диапазона
   * @param last итератор конеца диапазона
   */

  template<typename InputIterator>
  multiset(InputIterator first, InputIterator last)
    : super( first,  last)
  {
  }

  template<typename InputIterator>
  multiset(InputIterator beg, InputIterator end, const value_compare& comp, const allocator_type&  alloc= allocator_type() )
    : super( beg,  end, comp, alloc)
  {
  }

/*  multiset(const multiset& other)
    : super( other )
  {
  }
*/
  

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  multiset(const multiset&) = delete;
  
  multiset(multiset&& other)
    : super( std::move(other) )
  {
  }

  multiset( const std::initializer_list<value_type>& il, const value_compare& comp= value_compare(), const allocator_type&  alloc= allocator_type())
    : super( il, comp, alloc )
  {
  }

#endif

  multiset&  operator=(const multiset& other)
  {
    super::operator = (other);
    return *this;
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  multiset& operator=(multiset&& other)
  {
    super::operator = (std::move(other) );
    return *this;
  }

  multiset& operator=( std::initializer_list<value_type> il)
  {
    super::operator = (std::move(il) );
    return *this;
  }
#endif
};
}
#endif
