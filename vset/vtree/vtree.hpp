#ifndef VSET_VTREE_VTREE_HPP
#define VSET_VTREE_VTREE_HPP

#include <cstddef>
#include <stdexcept>
#include <map>
#include <limits>
#include <algorithm>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  #include <type_traits>
#endif

#include <vset/vtree/vtree_iterator.hpp>
#include <vset/vtree/aspect/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>

#include <fas/aop.hpp>
#include <fas/static_check/static_error.hpp>

namespace vset{ namespace vtree{

/// @cond ignore
template<typename K, typename C>
struct compare_pair
{
  C _comp;

  compare_pair()
    : _comp()
  {}
    
  explicit compare_pair(C comp)
    : _comp(comp)
  {
  }
    
  bool operator()(const K& first, const K& second) const
  {
    return _comp(first.first, second.first)
          || ( !_comp(second.first, first.first)
                && _comp(first.second, second.second) );
  }
};

/// @endcond


/**
   @brief Базовый класс для vset::multiset реализующий основные его методы.
   @tparam A - аспект
*/
template< typename A >
class vtree:
  public fas::aspect_class<A>
{
  typedef vtree<A> self;
  typedef fas::aspect_class<A> super;
  typedef typename super::aspect::template advice_cast<_allocator_>::type         allocator_builder;

  typedef typename super::aspect::template advice_cast<_key_compare_>::type       key_compare_tag;
  typedef typename super::aspect::template advice_cast<_value_compare_>::type     value_compare_tag;
  typedef typename super::aspect::template advice_cast<_value_type_>::type        value_type_tag;
  typedef typename super::aspect::template advice_cast<_key_type_>::type          key_type_tag;

  typedef typename allocator_builder::template apply<self>::type                  allocator_type_tag;
  typedef typename allocator_type_tag::size_type                                      size_type_tag;
  typedef typename allocator_type_tag::difference_type                                difference_type_tag;

  typedef std::pair<key_type_tag, key_type_tag>                                           container_key_tag;
  typedef compare_pair<container_key_tag, key_compare_tag>                        container_comparator_tag;

  typedef typename super::aspect
    ::template advice_cast<_container_>::type
    ::template apply<
       container_key_tag,
       typename allocator_type_tag::pointer, 
       container_comparator_tag
    >::type                                                                         container_type_tag;
    
  typedef vtree_iterator<typename container_type_tag::iterator, value_type_tag>             iterator_tag;
  typedef vtree_iterator<typename container_type_tag::const_iterator, const value_type_tag> const_iterator_tag;
  typedef std::reverse_iterator<iterator_tag>                                           reverse_iterator_tag;
  typedef std::reverse_iterator<const_iterator_tag>                                     const_reverse_iterator_tag;

  typedef typename std::iterator_traits<iterator_tag>::pointer                          pointer_tag;
  typedef typename std::iterator_traits<iterator_tag>::reference                        reference_tag;
  typedef typename std::iterator_traits<const_iterator_tag>::pointer                    const_pointer_tag;
  typedef typename std::iterator_traits<const_iterator_tag>::reference                  const_reference_tag;
public:

  typedef key_compare_tag key_compare;
  typedef value_compare_tag value_compare;
  typedef value_type_tag value_type;
  typedef key_type_tag key_type;

  typedef allocator_type_tag allocator_type;
  typedef size_type_tag size_type;
  typedef difference_type_tag difference_type;

  typedef container_key_tag container_key;
  typedef container_comparator_tag container_comparator;
  typedef container_type_tag container_type;
    
  typedef iterator_tag iterator;
  typedef const_iterator_tag const_iterator;
  typedef reverse_iterator_tag reverse_iterator;
  typedef const_reverse_iterator_tag const_reverse_iterator;

  typedef pointer_tag pointer;
  typedef reference_tag reference;
  typedef const_pointer_tag const_pointer;
  typedef const_reference_tag const_reference;
  
  
  allocator_type  _allocator;
  container_type  _container;

  // если есть _open_file_ копирование недоступно
  struct copy_ctor_disabled_for_mapped_files;

public:
  
  ~vtree()
  {
    this->clear();
  }

  /**
   * @brief Конструктор по умолчанию. Создаёт пустой контейнер.
   */
  vtree()
    : _allocator( this->get_aspect().template get<_allocator_>()( *this ) )
    , _container()
  {
  }

  /**
   * @brief Конструктор создаёт пустой контейнер. 
   * @param comp функции сравнения ключей
   * @param alloc функции сравнения ключей
   */
  explicit vtree(const key_compare& comp,const allocator_type& alloc = allocator_type() )
    : _allocator(alloc)
    , _container( container_comparator(comp) ) 
  {
    this->get_aspect().template get<_compare_>() = comp;
    _allocator = this->get_aspect().template get<_allocator_>()(*this);
  }

  /**
   * @brief Создаёт контейнер с содержимым из диапазона [first, last).
   * @param first итератор начала диапазона
   * @param last итератор конеца диапазона
   */
  template<typename InputIterator>
  vtree(InputIterator b, InputIterator e)
    : _allocator( this->get_aspect().template get<_allocator_>()(*this) )
    , _container()
  {
    this->insert(b, e);
  }

  /**
   * @brief Создаёт контейнер с содержимым из диапазона [first, last).
   * @param first итератор начала диапазона
   * @param last итератор конеца диапазона
   * @param comp функции сравнения ключей
   * @param alloc функции сравнения ключей
   */
  template<typename InputIterator>
  vtree(InputIterator b, InputIterator e, const value_compare& comp, const allocator_type&  alloc = allocator_type() )
    : _allocator(alloc)
    , _container( container_comparator(comp) ) 
  {
    this->get_aspect().template get<_compare_>() = comp;
    _allocator = this->get_aspect().template get<_allocator_>()(*this);
    this->insert(b, e);
  }
  
  /**
   * @brief Конструктор копирования
   * @details Для персистентных контейнеров копирование запрещено 
   */
  vtree(const self& __x)
    : _allocator( this->get_aspect().template get<_allocator_>()(*this) )
    , _container()
  {
    typedef typename fas::static_error< 
      copy_ctor_disabled_for_mapped_files, 
      super::aspect::template has_advice< ::vset::buffer::persistent::_open_file_ >::value == 0 
    >::type error;
    this->dummy( error() );
    this->insert(__x.begin(), __x.end());
  }
  

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  /**
    * @brief Move-конструктор (c++11). Создает контейнер с содержимым other с использованием move-семантики. 
    * Если alloc не был предоставлен, то он будет получен с помощью move-конструктора от аллокатора принадлежащего other.
    * @param other другой контейнер, который будет использован в качестве источника данных для инициализации элементов контейнера
    */  
  vtree(vtree&& __x)
  {
    vtree tmp;
    this->swap(__x);
  }

  /**
   * @brief Создает контейнер с содержимым списка инициализации init (c++11). 
   * @param init — список инициализации элементов контейнера
   * @param comp функции сравнения ключей
   * @param alloc функции сравнения ключей
   */
  vtree( std::initializer_list<value_type> il, const value_compare& comp = value_compare(), const allocator_type&  alloc = allocator_type())
    : _allocator(alloc)
    , _container( container_comparator(comp) ) 
  {
    this->get_aspect().template get<_compare_>() = comp;
    _allocator = this->get_aspect().template get<_allocator_>()(*this);
    this->insert(il.begin(), il.end());
  }

  vtree&& clone(const value_compare& comp, const allocator_type&  alloc = allocator_type()) const
  {
    return std::move(vtree( this->begin(), this->end(), comp, alloc));
  }

#endif

  vtree& operator=(const vtree& __x)
  {
    vtree tmp(__x);
    this->swap(tmp);
    return *this;
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  /**
   * @brief Заменяет содержимое содержимым other использованием семантики переноса (c++11)
   * @param other другой контейнер, который будет использоваться в качестве источника
   */
  vtree& operator=(vtree&& __x)
  {
    this->clear();
    this->swap(__x);
    return *this;
  }

  /**
   * @brief Заменяет содержимое элементами, которые указаны в списке идентификаторов инициализатора (c++11).
   * @param il список инициализации для использования в качестве источника данных
   */
  vtree& operator=( std::initializer_list<value_type> il)
  {
    this->clear();
    this->insert(il.begin(), il.end());
    return *this;
  }
#endif

  const key_compare& key_comp() const
  {
    return this->get_aspect().template get<_key_compare_>();
  }

  const value_compare& value_comp() const
  {
    return this->get_aspect().template get<_value_compare_>();
  }

  allocator_type get_allocator() const
  {
    return _allocator;
  }

  const container_type& get_container() const
  {
    return _container;
  }

  container_type& get_container()
  {
    return _container;
  }

  /** 
   * @brief Возвращает итератор на первый элемент контейнера. 
   * @return итератор на первый элемент
   */
  iterator  begin()
  {
    return iterator( _container.begin(), typename iterator::difference_type(0));
  }

  /** 
   * @brief Возвращает итератор на элемент, следующий за последним элементом контейнера
   * @return итератор на элемент, следующий за последним элементом контейнера
   * @details Этот элемент выступает в качестве заполнителя; попытке доступа к нему приводит к неопределенному поведению
   */  
  iterator end()
  {
    return iterator( _container.end(), typename  iterator::difference_type(0) );
  }

  /** 
   * @brief Возвращает константный итератор на первый элемент контейнера. 
   * @return Константный итератор на первый элемент
   */
  const_iterator  begin() const
  {
    return const_iterator( _container.begin(), 0);
  }

  /** 
   * @brief Возвращает константный итератор на элемент, следующий за последним элементом контейнера
   * @return константный  итератор на элемент, следующий за последним элементом контейнера
   * @details Этот элемент выступает в качестве заполнителя; попытке доступа к нему приводит к неопределенному поведению
   */  
  const_iterator end() const
  {
    return const_iterator( _container.end(), 0 );
  }

  /** 
   * @brief Возвращает обратный итератор на последний элемент контейнера. 
   * @return обратный итератор на последний элемент
   */
  reverse_iterator rbegin()
  {
    return reverse_iterator( this->end() );
  }

  /** 
   * @brief Возвращает обратный итератор на элемент, перед первым элементом контейнера
   * @return обратный  итератор на элемент, перед первым элементом контейнера
   * @details Этот элемент выступает в качестве заполнителя; попытке доступа к нему приводит к неопределенному поведению
   */  
  reverse_iterator rend()
  {
    return reverse_iterator( this->begin() );
  }

  /** 
   * @brief Возвращает константный обратный итератор на последний элемент контейнера. 
   * @return Константный обратный итератор на последний элемент
   */
  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator( this->end() );
  }

  /** 
   * @brief Возвращает константный обратный итератор на элемент, следующий перед первым элементом контейнера
   * @return константный  итератор обратный на элемент, следующий за последним элементом контейнера
   * @details Этот элемент выступает в качестве заполнителя; попытке доступа к нему приводит к неопределенному поведению
   */  
  const_reverse_iterator rend() const
  {
    return const_reverse_iterator( this->begin() );
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  /** 
   * @brief Возвращает константный итератор на первый элемент контейнера (c++11). 
   * @return Константный итератор на первый элемент
   */
  const_iterator  cbegin() const
  {
    return const_iterator( _container.cbegin(), 0);
  }

  /** 
   * @brief Возвращает константный итератор на элемент, следующий за последним элементом контейнера (c++11)
   * @return константный  итератор на элемент, следующий за последним элементом контейнера
   * @details Этот элемент выступает в качестве заполнителя; попытке доступа к нему приводит к неопределенному поведению
   */  
  const_iterator cend() const
  {
    return const_iterator( _container.cend(), 0 );
  }

  /** 
   * @brief Возвращает константный обратный итератор на последний элемент контейнера (c++11). 
   * @return Константный обратный итератор на последний элемент
   */
  const_reverse_iterator crbegin() const
  {
    return const_reverse_iterator( this->begin() );
  }


  /** 
   * @brief Возвращает константный обратный итератор на элемент, следующий перед первым элементом контейнера (c++11)
   * @return константный  итератор обратный на элемент, следующий перед первым элементом контейнера
   * @details Этот элемент выступает в качестве заполнителя; попытке доступа к нему приводит к неопределенному поведению
   */  
  reverse_iterator crend() const
  {
    return const_reverse_iterator( this->end() );
  }

#endif

  /**
   * @brief Проверка на отсутствие элементов в контейнере 
   * @return true если контейнер пуст, false - в противном случае 
   */
  bool empty() const
  {
    return _container.empty();
  }

  /**
   * @brief Возвращает количество элементов в контейнере
   * @return количество элементов в контейнере
   */
  size_type size() const
  {
    return this->get_aspect().template get<_size_>();
  }

  /**
   * @brief Возвращает максимально допустимое количество элементов в контейнере 
   * @return Максимальное количество элементов.
   * @details Это значение обычно равно std::numeric_limits<size_type>::max(), и отражает теоретический предел на размер контейнера. 
   * Ввиду ограничений на доступную оперативную память, во время исполнения это значение может быть ниже чем max_size().
   */
  size_type max_size() const
  {
    return std::numeric_limits<size_type>::max();
  }

  /**
   * @brief Обменивает содержимое контейнера с содержимым контейнера другого
   * @param other — Контейнер для обмена содержимым
   * @details  The behavior is undefined if get_allocator() != other.get_allocator() 
   * and std::allocator_traits<allocator_type>::propagate_on_container_swap::value != true.
   */
  void swap( vtree& other )
  {
    this->get_aspect().template get<_swap_container_>()(*this, other);
    std::swap(_allocator, other._allocator);
    /*allocator_type old_alloc = _allocator;
    _allocator = this->get_aspect().template get<_allocator_>()(*this);
    other._allocator = this->get_aspect().template get<_allocator_>()(other);*/
  }

  /**
   * @brief Возвращает количество элементов, которые могут одновременно храниться в выделенной области памяти 
   * @return Вместимость контейнера, под которую в сейчас выделена память
   */
  size_t capacity() const
  {
    return this->get_aspect().template get<_capacity_>()(*this);
  }
  
  /**
   * @brief Вставляет элемент
   * @param value вставляемое значение
   * @return итератор на вставленный элемент
   * @details Сложность от O(log(size/t)) до O(log(size*4/t) + t)
   */
  iterator insert(const value_type& value)
  {
    return this->get_aspect().template get<_insert_value_>()(*this, value);
  }


#ifdef __GXX_EXPERIMENTAL_CXX0X__

  /**
   * @brief Вставляет элемент (c++11)
   * @param value вставляемое значение
   * @return итератор на вставленный элемент
   * @details Сложность от O(log(size/t)) до O(log(size*4/t) + t)
   */
  iterator insert(value_type&& value)
  {
    return this->get_aspect().template get<_insert_value_>()(*this, value );
  }

#endif

  /**
   * @brief Вставляет элемент
   * @param const_iterator игнорируеться
   * @param value вставляемое значение
   * @return итератор на вставленный элемент
   * @details Сложность от O(log(size/t)) до O(log(size*4/t) + t)
   */
  iterator insert(const_iterator, const value_type& value)
  {
    return this->get_aspect().template get<_insert_value_>()(*this, value );
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
  /**
   * @brief Вставляет элемент (c++11)
   * @param const_iterator игнорируеться
   * @param value вставляемое значение
   * @return итератор на вставленный элемент
   * @details Сложность от O(log(size/t)) до O(log(size*4/t) + t)
   */
  iterator insert(const_iterator, value_type&& value)
  {
    return this->get_aspect().template get<_insert_value_>()(*this, value );
  }
#endif

  /**
   * @brief Вставляет элементы из диапазона [first, last)
   * @param first итератор начала диапазона
   * @param last итератор конеца диапазона
   */
  template<typename InputIterator>
  void insert(InputIterator first, InputIterator last)
  {
    for (; first != last; ++first)
    {
      this->insert(*first);
    }
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  /**
   * @brief Вставляет элементы из списка инициализации init (c++11)
   * @param init — список инициализации элементов контейнера
   */
  void insert( std::initializer_list<value_type> init)
  {
    this->insert(init.begin(), init.end());
  }

#endif

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  /**
   * @brief Удаляет элементы (c++11)
   * @param pos итератор на элемент для удаления
   */
  iterator erase(const_iterator pos)
  {
    return this->get_aspect().template get<_erase_iterator_>()(*this, pos);
  }

#else

  /**
   * @brief Удаляет элементы (c++03)
   * @param pos итератор на элемент для удаления
   */
  void erase(iterator itr)
  {
    this->get_aspect().template get<_erase_iterator_>()(*this, itr);
  }

#endif

  /**
   * @brief Удаляет элементы (c++11)
   * @param key	Ключевое значение элементов для удаления
   */
  size_type erase(const key_type& key)
  {
    return this->get_aspect().template get<_erase_value_>()(*this, key);
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  /**
   * @brief Удаляет элементы (c++11) из диапазона [first, last)
   * @param first итератор начала диапазона
   * @param last итератор конеца диапазона
   * @return iterator, следующего за последним удаленным элементом
   */
  iterator erase(const_iterator first, const_iterator last)
  {
    return this->get_aspect().template get<_erase_range_>()(*this, first, last);
  }

#else

  /**
   * @brief Удаляет элементы (c++03) из диапазона [first, last)
   * @param first итератор начала диапазона
   * @param last итератор конеца диапазона
   */
  void erase(iterator first, iterator last)
  {
    this->get_aspect().template get<_erase_range_>()(*this, first, last);
  }

#endif

  /** @brief Очищает контейнер */
  void clear()
  {
    return this->get_aspect().template get<_clear_>()(*this);
  }

  /** 
   * @brief Возвращает количество элементов с ключом key.
   * @param key Значение ключа
   * @return Количество элементов с ключом key
   */
  size_type count(const key_type& key) const
  {
    return static_cast<size_type>( this->upper_bound(key) - this->lower_bound(key) );
  }

#ifdef __GXX_EXPERIMENTAL_CXX0X__

  /** 
   * @brief находит элемент с конкретным ключом (c++11)
   * @param key Ключевое значение элемента для поиска
   * @return Итератор элемент с ключом key. Если такой элемент не найден, итератора возвращается vtree::end()
   */
  iterator find(const key_type& key)
  {
    return this->get_aspect().template get<_find_>()(*this, key);
  }

  /** 
   * @brief находит элемент с конкретным ключом (c++11)
   * @param key Ключевое значение элемента для поиска
   * @return Итератор элемент с ключом key. Если такой элемент не найден, итератора возвращается vtree::end()
   */
  const_iterator find(const key_type& key ) const
  {
    return this->get_aspect().template get<_find_>()(*this, key);
  }

  /** 
   * @brief возвращает итератор на первый элемент не меньшим, чем заданное значение (c++11)
   * @param key ключевое значение для сравнения элементов
   * @return итератор, указывающий на первый элемент, который не меньше, чем key. Если такой элемент не найден, итератора возвращается vtree::end()
   */
  iterator lower_bound(const key_type& key)
  {
    return this->get_aspect().template get<_lower_bound_>()(*this, key);
  }

  /** 
   * @brief возвращает итератор на первый элемент не меньшим, чем заданное значение (c++11)
   * @param key ключевое значение для сравнения элементов
   * @return итератор, указывающий на первый элемент, который не меньше, чем key. Если такой элемент не найден, итератора возвращается vtree::end()
   */
  const_iterator lower_bound(const key_type& key) const
  {
    return this->get_aspect().template get<_lower_bound_>()(*this, key);
  }

  /** 
   * @brief возвращает итератор на первый элемент больше, чем заданное значение (c++11)
   * @param key ключевое значение для сравнения элементов
   * @return итератор, указывающий на первый элемент, который больше, чем key. Если такой элемент не найден, итератора возвращается vtree::end()
   */
  iterator upper_bound(const key_type& key)
  {
    return this->get_aspect().template get<_upper_bound_>()(*this, key);
  }

  /** 
   * @brief возвращает итератор на первый элемент больше, чем заданное значение (c++11)
   * @param key ключевое значение для сравнения элементов
   * @return итератор, указывающий на первый элемент, который больше, чем key. Если такой элемент не найден, итератора возвращается vtree::end()
   */
  const_iterator upper_bound(const key_type& key) const
  {
    return this->get_aspect().template get<_upper_bound_>()(*this, key);
  }

#else

  /** 
   * @brief находит элемент с конкретным ключом (c++03)
   * @param key Ключевое значение элемента для поиска
   * @return Итератор элемент с ключом key. Если такой элемент не найден, итератора возвращается vtree::end()
   */
  iterator find(const key_type& key) const
  {
    self* t = const_cast<self*>(this);
    return t->get_aspect().template get<_find_>()(*t, key);
  }

  /** 
   * @brief возвращает итератор на первый элемент не меньшим, чем заданное значение (c++03)
   * @param key ключевое значение для сравнения элементов
   * @return итератор, указывающий на первый элемент, который не меньше, чем key. Если такой элемент не найден, итератора возвращается vtree::end()
   */
  iterator lower_bound(const key_type& key) const
  {
    self* t = const_cast<self*>(this);
    return t->get_aspect().template get<_lower_bound_>()(*t, key);
  }

  /** 
   * @brief возвращает итератор на первый элемент больше, чем заданное значение (c++03)
   * @param key ключевое значение для сравнения элементов
   * @return итератор, указывающий на первый элемент, который больше, чем key. Если такой элемент не найден, итератора возвращается vtree::end()
   */
  iterator upper_bound(const key_type& key) const
  {
    self* t = const_cast<self*>(this);
    return t->get_aspect().template get<_upper_bound_>()(*t, key);
  }

#endif

  /** 
   * @brief Возвращает диапазон, содержащий все элементы с ключевыми key в контейнере
   * @param key ключевое значение для сравнения элементов
   * @return Пара (std::pair) итераторов, определяющих требуемый диапазон: первый указывает на первый элемент, который не меньше ключа, 
   * а второй указывает на первый элемент, кторый больше ключа.
   */
  std::pair<iterator, iterator> equal_range(const key_type& x)
  {
    return std::make_pair(this->lower_bound(x), this->upper_bound(x) );
  }

  /** 
   * @brief Возвращает диапазон, содержащий все элементы с ключевыми key в контейнере
   * @param key ключевое значение для сравнения элементов
   * @return Пара (std::pair) итераторов, определяющих требуемый диапазон: первый указывает на первый элемент, который не меньше ключа, 
   * а второй указывает на первый элемент, кторый больше ключа.
   */
  std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const
  {
    return std::make_pair(this->lower_bound(x), this->upper_bound(x) );
  }


  template<typename AA>
  friend bool operator==(const vtree<AA>&, const vtree<AA>&);

  template<typename AA>
  friend bool operator<(const vtree<AA>&, const vtree<AA>&);
  
private:
  
  
  template<typename T>
  void dummy(T){}

};

template<typename A>
inline bool operator==(const vtree<A>& __x, const vtree<A>& __y)
{
  return !(__x < __y) && !(__y < __x);
  //return __x.size() == __y.size() && std::equal(__x.begin(), __x.end(), __y.begin());
}

template<typename A>
inline bool operator< (const vtree<A>& __x, const vtree<A>& __y)
{
  return std::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end(), __x.key_comp() );
}

template<typename A>
inline bool
operator!=(const vtree<A>& __x, const vtree<A>& __y)
{
  return !(__x == __y);
}

template<typename A>
inline bool operator>(const vtree<A>& __x, const vtree<A>& __y)
{
  return __y < __x;
}

template<typename A>
inline bool operator<=(const vtree<A>& __x, const vtree<A>& __y)
{
  return !(__y < __x);
}

template<typename A>
inline bool operator>=(const vtree<A>& __x, const vtree<A>& __y)
{
  return !(__x < __y);
}

template<typename A>
inline void swap(vtree<A>& __x, vtree<A>& __y)
{
  __x.swap(__y);
}

}}

#endif
