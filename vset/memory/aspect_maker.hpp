//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_MEMORY_ASPECT_MAKER_HPP
#define VSET_MEMORY_ASPECT_MAKER_HPP

#include <vset/memory/tags.hpp>
#include <iterator>

namespace vset { namespace memory{

template< typename C >
struct aspect_maker
{
private:
  typedef C aspect_class;
  
  typedef typename aspect_class::aspect::template advice_cast<_buffer_provider_>::type
                                ::type buffer_class;

  typedef typename aspect_class::aspect::template advice_cast<_pointer_provider_>::type
                                ::type pointer_class;
public:
  
  typedef typename aspect_class::aspect::template advice_cast<_value_type_>::type value_type;

  typedef typename fas::apply<buffer_class, aspect_class>::type buffer_type;
  typedef typename fas::apply<pointer_class, value_type, aspect_class >::type pointer;
  typedef typename fas::apply<pointer_class, const value_type, const aspect_class >::type const_pointer;
  typedef typename std::iterator_traits<value_type*>::difference_type   difference_type;
};

}}

#endif
