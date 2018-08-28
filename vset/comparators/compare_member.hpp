//
// Author: Vladimir Migashko <migashko@wamba.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef COMPARATORS_COMPARE_MEMBER_HPP
#define COMPARATORS_COMPARE_MEMBER_HPP

# include <vset/comparators/compare.hpp>
#include <fas/functional/member.hpp>
#include <functional>

namespace vset{

/**
 * @brief Позволяет создавать компаратор отдельного поля структуры или класса
 * @tparam S - тип структуры 
 * @tparam M - тип поля структуры
 * @tparam m - указатель на поле структуры
 * @tparam C - копаратор для M (по умолчанию std::less<M> )
 */  
template<typename S, typename M, M S::* m, typename C = std::less<M> >
struct compare_member: compare< fas::member<S, M, m>, C > {};

}

#endif
