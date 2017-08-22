//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2017
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_AD_POS2OFF_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_AD_POS2OFF_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>
#include <cstddef>

namespace vset { namespace memory{ namespace fsb{

struct ad_pos2off
{
  template<typename T>
  size_t operator()(T&, size_t pos) const
  {
    typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
    typedef typename T::aspect::template advice_cast<_chunk_type_>::type chunk_type;
    typedef typename T::aspect::template advice_cast<_chain_type_>::type chain_type;

    size_t real_offset = chain_type::head_size();
    real_offset += chunk_type::head_size() + (pos / chunk_type::max_count()) * chunk_type::head_size();
    real_offset += sizeof(value_type)*pos;
    return real_offset;
  }
};

struct ad_pos2off_stub
{
  template<typename T>
  size_t operator()(T&, size_t pos) const
  {
    return pos;
  }
};

}}}

#endif
