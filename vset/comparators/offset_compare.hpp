//
// Author: Vladimir Migashko <migashko@wamba.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef COMPARATORS_OFFSET_COMPARE_HPP
#define COMPARATORS_OFFSET_COMPARE_HPP

namespace vset{
  
/**
 * @brief Обертка компаратора для сравнения целочисленных смещений менеджера памяти
 * @tparam O смещение (обычно size_t, uint32_t или uint16_t)
 * @tparam M менеджер памяти (vset::memory::fsb_mmap, vset::memory::fsb_inmem)
 * @tparam C компаратор для исходных типов 
 */
template<typename O, typename M, typename C>
class offset_compare: C
{
public:
  typedef C super;
  typedef O offset_t;
  typedef M memory_manager_type;
  typedef typename memory_manager_type::pointer pointer_type;

  /**
    * @brief Конструктор создает неинициализированный объект, кторый нельзя использовать для сравнения
    */
  offset_compare()
    : lvp(0)
    , rvp(0)
  {
  }

  /**
    * @brief Конструктор 
    * @param ptr указатель на любой объект менеджера памяти, в том числе и, например, vset::memory::fsb_mmap::end()
    */
  explicit offset_compare( pointer_type ptr )
    : lvp(ptr)
    , rvp(ptr)
  {
  }

  /**
    * @brief Оператор сравнения
    * @param left значение для сравнения
    * @param right значение для сравнения
    */
  bool operator() ( offset_t left, offset_t right ) const
  {
    lvp.set_offset( left );
    rvp.set_offset( right );
    return super::operator()(*lvp, *rvp);
  }

private:

  mutable pointer_type lvp;
  mutable pointer_type rvp;
};

}

#endif
