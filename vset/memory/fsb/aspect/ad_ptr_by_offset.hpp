//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_MEMORY_FSB_ASPECT_AD_GET_BY_OFFSET_HPP
#define VSET_VSET_MEMORY_FSB_ASPECT_AD_GET_BY_OFFSET_HPP

#include <vset/buffer/tags.hpp>
#include <vset/memory/fsb/tags.hpp>

namespace vset { namespace memory{ namespace fsb{

struct ad_ptr_by_offset
{
  template<typename T>
  typename T::aspect::template advice_cast<_value_type_>::type*
  operator()(T& t, size_t offset) const
  {
    typedef ::vset::buffer::_data_      _buffer_data_;
    typedef ::vset::buffer::_data_type_ _buffer_data_type_;

    typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
    typedef typename T::aspect::template advice_cast<_buffer_data_type_>::type data_type;
    typedef typename T::aspect::template advice_cast<_chunk_type_>::type chunk_type;
    typedef typename T::aspect::template advice_cast<_chain_type_>::type chain_type;

    std::cout << "!ad_ptr_by_offset " << offset;
    size_t real_offset = offset * sizeof(value_type);
    std::cout << ", " << real_offset ;
    real_offset += offset/chunk_type::max_count();
    std::cout << ", " << real_offset ;
    real_offset += chain_type::head_size();
    std::cout << ", " << real_offset ;
    real_offset += chunk_type::head_size();
    std::cout << " >> " << real_offset << std::endl;
    data_type data = t.get_aspect().template get<_buffer_data_>()(t);
    return reinterpret_cast<value_type*>(data + real_offset);
    /*
    data_type data = t.get_aspect().template get<_buffer_data_>()(t);
    char* ptr = reinterpret_cast<char*>(data);
    ptr += chain_type::head_size();
    ptr += chunk_type::head_size();
    ptr += offset * sizeof(value_type);
    ptr += (offset / chunk_type::max_count())*chunk_type::head_size();
    return reinterpret_cast<value_type*>(ptr);
    */
  }
};
  
struct ad_ptr_by_offset2
{
  template<typename T>
  typename T::aspect::template advice_cast<_value_type_>::type*
  operator()(T& t, size_t offset) const
  {
    typedef ::vset::buffer::_data_      _buffer_data_;
    typedef ::vset::buffer::_data_type_ _buffer_data_type_;

    typedef typename T::aspect::template advice_cast<_value_type_>::type value_type;
    typedef typename T::aspect::template advice_cast<_buffer_data_type_>::type data_type;

    data_type data = t.get_aspect().template get<_buffer_data_>()(t);
    return reinterpret_cast<value_type*>(data + offset);
  }
};

}}}

#endif
