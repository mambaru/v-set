//
// Author: Vladimir Migashko <migashko@wamba.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef COMPARATORS_POINTER_COMPARE_HPP
#define COMPARATORS_POINTER_COMPARE_HPP

namespace vset{
  
/**
 * @brief Обертка компаратора для сравнения указателей или других типов с определенным `operator *`
 * @tparam C компаратор 
 */
template<typename C>
class pointer_compare: C
{
public:
  /**
   * @brief оператор сравнения
   * @tparam D тип исходных данных (например указатель или итератор)
   * @param left значение для сравнения
   * @param right значение для сравнения
   * @return `C::operator()( *left, *right)`
   */
  template<typename D>
  bool operator()(const D& left, const D& right) const
  {
    return C::operator()( *left, *right);
  }
};

}

#endif
