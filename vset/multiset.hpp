#ifndef VSET_MULTI_SET_HPP
#define VSET_MULTI_SET_HPP

#include <fas/typemanip/has_typename.hpp>
#include <vset/vtree/vtree.hpp>
#include <vset/vtree/strategy.hpp>

namespace vset{ 

template<typename V, typename C = std::less<V>, typename A = std::allocator<V> >
class multiset;
  
/** @internal*/
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

  
  template<typename V, typename C, typename A>
  struct multiset
  {
    typedef typename multiset_impl<V, C, A, aspect_maker::is_aspect_maker<A>::value >::type type;
  };
}


/**
   @brief ассоциативный контейнер vset::multiset содержит упорядоченный набор объектов типа V (допускаются ключи с одинаковыми значениями).
          Сортировка производится с помощью функции сравнения ключей Compare. Операции поиска, вставки и удаления имеют отличную от `std::multiset` 
          логорифмичскую сложность и определяется стратегией хранения, которая внедряется через аллокатор.
   @tparam V - тип хранимых значений 
   @tparam C - функции сравнения ключей
   @tparam A - аллокатор
   
*/
template<typename V, typename C, typename A >
class multiset
  : public aspect_maker::multiset<V, C, A >::type
{
  typedef typename aspect_maker::multiset<V, C, A >::type super;
  
public:
  /** Key: _key_type_ */
  typedef typename super::key_type    key_type;
  /** Key: _value_type_ */
  typedef typename super::value_type  value_type;
  /** Compare: _key_compare_ */
  typedef typename super::key_compare    key_compare;
  /** Compare: _value_compare_ */
  typedef typename super::value_compare  value_compare;
  /** Allocator */
  typedef typename super::allocator_type allocator_type;
  /** std::iterator_traits<iterator>::reference */
  typedef typename super::reference reference;
  /** std::iterator_traits<iterator>::const_reference */
  typedef typename super::const_reference const_reference;
  /** std::iterator_traits<iterator>::pointer */
  typedef typename super::pointer pointer;
  /** std::iterator_traits<iterator>::const_pointer */
  typedef typename super::const_pointer const_pointer;
  /** Итератор произвольного доступа */
  typedef typename super::iterator iterator;
  /** Константный итератор произвольного доступа */
  typedef typename super::const_iterator const_iterator;
  /** std::reverse_iterator<iterator> */
  typedef typename super::reverse_iterator reverse_iterator;
  /** std::reverse_iterator<const_iterator> */
  typedef typename super::const_reverse_iterator const_reverse_iterator;
  /** Знаковый целочисленный тип (обычно std::ptrdiff_t) */
  typedef typename super::difference_type difference_type;
  /** Беззнаковый целочисленный тип (обычно size_t) */
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

  /**
   * @brief Создаёт контейнер с содержимым из диапазона [first, last).
   * @param first итератор начала диапазона
   * @param last итератор конеца диапазона
   * @param comp функции сравнения ключей
   * @param alloc функции сравнения ключей
   */
  template<typename InputIterator>
  multiset(InputIterator first, InputIterator last, const value_compare& comp, const allocator_type&  alloc= allocator_type() )
    : super( first,  last, comp, alloc)
  {
  }

 

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  /**
   * @brief Копирование запрещено (c++11)
   */
  multiset(const multiset&) = delete;

/**
 * @brief Move-конструктор (c++11). Создает контейнер с содержимым other с использованием move-семантики. Если alloc не был предоставлен, то он будет получен 
 *        с помощью move-конструктора от аллокатора принадлежащего other.
 * @param other другой контейнер, который будет использован в качестве источника данных для инициализации элементов контейнера
 */  
  multiset(multiset&& other)
    : super( std::move(other) )
  {
  }

  /**
   * @brief Создает контейнер с содержимым списка инициализации init (c++11). 
   * @param init — список инициализации элементов контейнера
   * @param comp функции сравнения ключей
   * @param alloc функции сравнения ключей
   */
  multiset( const std::initializer_list<value_type>& init, const value_compare& comp= value_compare(), const allocator_type&  alloc= allocator_type())
    : super( init, comp, alloc )
  {
  }

#endif

  /**
   * @brief Заменяет содержимое копией содержимого other
   * @param other другой контейнер, который будет использоваться в качестве источника
   */
  multiset&  operator=(const multiset& other)
  {
    super::operator = (other);
    return *this;
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  /**
   * @brief Заменяет содержимое содержимым other использованием семантики переноса (c++11)
   * @param other другой контейнер, который будет использоваться в качестве источника
   */
  multiset& operator=(multiset&& other)
  {
    super::operator = (std::move(other) );
    return *this;
  }

  /**
   * @brief Заменяет содержимое элементами, которые указаны в списке идентификаторов инициализатора (c++11).
   * @param il список инициализации для использования в качестве источника данных
   */
  multiset& operator=( std::initializer_list<value_type> il)
  {
    super::operator = (std::move(il) );
    return *this;
  }
  
#endif
};

}
#endif
