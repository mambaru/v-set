//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_ALLOCATOR_CHAIN_ALLOCATOR_HPP
#define VSET_VSET_ALLOCATOR_CHAIN_ALLOCATOR_HPP

#include <cstddef>
#include <vset/memory/manager_base.hpp>
#include <vset/memory/strategy.hpp>
#include <fas/system/nullptr.hpp>

namespace vset { namespace memory{

/**
 * @brief Интерфейс менеждера памяти
 * @tparam S стратегия 
 * @tparam A дополнительный аспект
 * @details доступные стратегии:
 *    * vset::memory::strategy::fsb_mmap
 *    * vset::memory::strategy::fsb_inmem
 *    * vset::memory::strategy::fsb_filesync
 */
template<typename S, typename A = fas::aspect<> >
class manager
  : public manager_base< typename fas::merge_aspect<A, S>::type >
{
  typedef manager_base< typename fas::merge_aspect<A, S>::type > super;
public:
  typedef manager<S, A> self;
  /** Тип размещаемого объекта */
  typedef typename super::value_type    value_type;
  /** Буфер, где рамещаются объекты value_type*/
  typedef typename super::buffer_type   buffer_type;
  /** Указатель vset::memory::offset_pointer */
  typedef typename super::pointer       pointer;
  /** Константный vset::memory::offset_pointer */
  typedef typename super::const_pointer const_pointer;
  
  typedef value_type& reference;
  typedef const value_type& const_reference;

  /** Беззнаковый целочисленный тип (size_t) */
  typedef size_t size_type;
  /** Знаковый целочисленный тип (std::ptrdiff_t) */
  typedef std::ptrdiff_t difference_type;

  /** 
   * @brief Возвращает указатель на первый элемент контейнера. 
   * @return указатель vset::memory::offset_pointer на первый элемент
   */
  pointer begin()
  {
    return super::_begin(*this);
  }

  /** 
   * @brief Возвращает указатель на первый элемент контейнера. 
   * @return указатель vset::memory::offset_pointer на первый элемент
   */
  const_pointer begin() const
  {
    return super::_begin(*this);
  }

  /** 
   * @brief Возвращает указатель на элемент, следующий за последним элементом контейнера
   * @return указатель vset::memory::offset_pointer на элемент, следующий за последним элементом контейнера
   * @details Этот элемент выступает в качестве заполнителя; попытке доступа к нему приводит к неопределенному поведению
   */  
  pointer end()
  {
    return super::_end(*this);
  }

  /** 
   * @brief Возвращает указатель на элемент, следующий за последним элементом контейнера
   * @return указатель vset::memory::offset_pointer на элемент, следующий за последним элементом контейнера
   * @details Этот элемент выступает в качестве заполнителя; попытке доступа к нему приводит к неопределенному поведению
   */  
  const_pointer end() const
  {
    return super::_end(*this);
  }

  /**
   * @brief Выделяет память для num объектов
   * @param num количество объектов
   * @return указатель vset::memory::offset_pointer на первый объект
   * @details указатель на первый объект не является указателем на первый элемент массива,
   * но получить доступ к остальным объектам можно с помощью операторов += и ++. Но основной 
   * кейс это выделение и освобождение по одному объекту
   */
  pointer allocate(size_t num, void *  hint = fas_nullptr)
  {
    return super::_allocate(*this, num, hint);
  }

  /**
   * @brief Освобождает память для num объектов
   * @param num количество объектов
   * @details освобождает память для текущего объекта и еще для num-1 следующих. Но основной 
   * кейс это выделение и освобождение по одному объекту
   */
  void deallocate(pointer ptr, size_type num)
  {
    return super::_deallocate(*this, ptr, num);
  }

  /**
   * @brief Возвращает количество распределенных объектов
   * @return количество распределенных объектов
   */
  size_type count() const
  {
    return super::_count(*this);
  }
  
  /**
   * @brief Проверка на отсутствие элементов 
   * @return true если память ни для одного элемента не распределена, false - в противном случае 
   */
  bool empty() const
  {
    return this->begin() == this->end();
  }

  /**
   * @brief Возвращает количество элементов, для которого зарезервирована память контейнером.
   * @return Текущая вместимость, под которую в сейчас выделена память
   */
  size_type capacity() const
  {
    return super::_capacity(*this);
  }
  
  /**
   * @brief Буфер
   * @return Объект управления буфером в котором происходит размещение объектов
   */
  buffer_type buffer()
  {
    return super::_buffer(*this);
  }

  /**
   * @brief Буфер
   * @return Объект управления буфером в котором происходит размещение объектов
   */
  const buffer_type buffer() const
  {
    return super::_buffer(*this);
  }

};

}}

#endif
