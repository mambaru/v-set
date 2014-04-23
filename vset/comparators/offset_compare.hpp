//
// Author: Vladimir Migashko <migashko@wamba.com>, (C) 2013
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef COMPARATORS_OFFSET_COMPARE_HPP
#define COMPARATORS_OFFSET_COMPARE_HPP

namespace vset{
  
template<typename O, typename M, typename C>
class offset_compare: C
{
public:
  typedef C super;
  typedef O offset_t;
  typedef M memory_manager_type;
  typedef typename memory_manager_type::pointer pointer_type;

  offset_compare()
    : lvp(0)
    , rvp(0)
  {
  }

  offset_compare( pointer_type ptr )
    : lvp(ptr)
    , rvp(ptr)
  {
  }

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
