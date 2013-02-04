#include <iostream>
#include <string>
#include <ctime>
#include <stdint.h>

#include <vset/vtree/vtree.hpp>
#include <vset/memory/manager.hpp>


namespace rate {

#define NAME_LENGTH 256

typedef uint32_t  offset_t;

typedef uint32_t  id_t;

typedef id_t      rating_id_t;
typedef uint32_t  rating_type_t;
typedef uint32_t  top_count_t;
typedef uint8_t   gender_t;
typedef uint32_t  priority_t;
 

struct Rating
{
  rating_id_t       id;
  char              name[NAME_LENGTH];
  rating_type_t     rating_type;
  top_count_t       top_count;
  time_t            start_date;
  time_t            finish_date;
  bool              is_on_first;
  bool              is_adult;
  gender_t          gender;
  bool              is_deleted;
  priority_t        priority;

  bool
  operator() ( Rating const & r )
  {
    return this->id < r.id;
  }

}; // Rating


typedef vset::vtree::vtree< vset::vtree::aspect<Rating, std::less<Rating>, 512> > rating_tree_t;


} // ns rate



int main()
{
  rate::rating_tree_t rating;

  

  return 0;
}


