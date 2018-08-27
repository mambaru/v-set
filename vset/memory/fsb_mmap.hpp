//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2017
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMMORY_FSB_MMAP_HPP
#define VSET_VSET_MEMMORY_FSB_MMAP_HPP

#include <vset/memory/strategy.hpp>
#include <vset/memory/manager.hpp>
#include <fas/aop.hpp>

namespace vset { namespace memory{

/**
  * @brief менеджер памяти на файле отображенном в память 
  * @tparam T тип хранимых объектов
  * @tparam OffsetAspect стратегия смещения. По умолчанию vset::memory::fsb_offset
  * @details также доступна стратегия относительного смещения vset::memory::fsb_nth, 
  * которая работает по смещению не в байтах относительно начала буффера как vset::memory::fsb_offset,
  * а по количеству объектов T, что позволит проиндексировать в uint32_t гораздо больше элементов.
  */
template<typename T, typename OffsetStrategy = fsb_offset, typename A = fas::aspect<> >
struct fsb_mmap
  : manager< strategy::fsb_mmap<T, OffsetStrategy>, A >
{};

}}

#endif
