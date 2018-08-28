//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMMORY_STRATEGY_HPP
#define VSET_VSET_MEMMORY_STRATEGY_HPP

#include <vset/memory/fsb/aspect.hpp>
#include <vset/memory/fsb/aspect/aspect_pointer.hpp>
#include <vset/buffer/persistent/mmap/aspect.hpp>
#include <vset/buffer/persistent/filesync/aspect.hpp>
#include <vset/buffer/inmem/aspect.hpp>
#include <vset/buffer/persistent_provider.hpp>
#include <vset/buffer/provider.hpp>

namespace vset { namespace memory{

/** @brief стратегия абсолютного смещения */
struct fsb_offset: fsb::aspect_offset{};

/** @brief стратегия относительного смещения */
struct fsb_nth: fsb::aspect_nth{};

namespace strategy
{

  /** 
   * @brief стратегия менеджера памяти на файле отображенном в память
   * @tparam T тип объекта размещения
   * @tparam OffsetStrategy стратегия смещения 
   */
  
  template<typename T, typename OffsetStrategy = fsb_offset >
  struct fsb_mmap
    : fsb::aspect<
        T,
        OffsetStrategy,
        vset::buffer::persistent::mmap::aspect,
        buffer::persistent_provider
      >
  {};

  /** 
   * @brief стратегия менеджера памяти на непрерывном буфере с поддержкой полного дампма в файл 
   * @tparam T тип объекта размещения
   * @tparam OffsetStrategy стратегия смещения 
   */
  template<typename T, typename OffsetStrategy = fsb_offset >
  struct fsb_filesync
    : fsb::aspect<
        T,
        OffsetStrategy,
        vset::buffer::persistent::filesync::aspect,
        buffer::persistent_provider
      >
  {};

  /** 
   * @brief стратегия менеджера памяти на непрерывном буфере 
   * @tparam T тип объекта размещения
   * @tparam OffsetStrategy стратегия смещения 
   */
  template<typename T, typename OffsetStrategy = fsb_offset >
  struct fsb_inmem
    : fsb::aspect<
        T,
        OffsetStrategy,
        vset::buffer::inmem::aspect,
        buffer::provider
      >
  {};
}
  
}}

#endif
