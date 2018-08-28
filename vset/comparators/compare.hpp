//
// Author: Vladimir Migashko <migashko@wamba.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef COMPARATORS_COMPARE_HPP
#define COMPARATORS_COMPARE_HPP

namespace vset{
  
/**
 * @brief Компаратор с преобразованием 
 * @tparam A преобразователь
 * @tparam C компаратор 
 */
template<typename A, typename C>
struct compare
{
  /**
   * @brief оператор сравнения
   * @tparam D тип исходных данных (например указатель или итератор)
   * @param left значение для сравнения
   * @param right значение для сравнения
   * @return `C()( A()(left), A()(right) )`
   */
  template<typename D>
  bool operator()(const D& left, const D& right) const
  {
    return C()( A()(left), A()(right) );
  }
};

}

#endif
