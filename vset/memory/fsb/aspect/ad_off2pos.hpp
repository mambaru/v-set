//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2017
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_AD_OFF2POS_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_AD_OFF2POS_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>
#include <cstddef>

namespace vset { namespace memory{ namespace fsb{

struct ad_off2pos
{
  template<typename T>
  size_t operator()(T&, size_t offset) const
  {
    typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
    typedef typename T::aspect::template advice_cast<_chunk_type_>::type chunk_type;
    typedef typename T::aspect::template advice_cast<_chain_type_>::type chain_type;
    offset -= chain_type::head_size();
    offset -= ( offset/chunk_type::size() ) * chunk_type::head_size();
    offset -= chunk_type::head_size();
    return offset/sizeof(value_type);
  }
};

struct ad_off2pos_stub
{
  template<typename T>
  size_t operator()(T&, size_t offset) const
  {
    return offset;
  }
};


}}}

#endif
