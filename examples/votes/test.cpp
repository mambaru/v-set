#include <iostream>
#include <string>
#include <ctime>
#include <cstring>
#include <stdint.h>
#include <algorithm> // std::move

#include <vset/vtree/vtree.hpp>
#include <vset/memory/manager.hpp>


namespace rate {

#define NAME_LENGTH 256
#define VSET_VECTOR_LENGTH 512

typedef uint32_t  offset_t;

typedef uint32_t  id_t;
typedef id_t      rating_id_t;
typedef uint32_t  rating_type_t;
typedef uint32_t  top_count_t;
typedef uint8_t   gender_t;
typedef uint32_t  priority_t;
 

struct Rating
{
  Rating ()
  {
    this->id = 0;
  }

  Rating ( rating_id_t rid )
  {
    this->id = rid;
  }

  Rating ( Rating && r )
    : id( std::move( r.id ) )
  {
  }

  Rating ( Rating const & r )
  {
    *this = r;
  }

  Rating &
  operator= ( Rating const & r )
  {
    this->id = r.id;
    std::strncpy( this->name, r.name, NAME_LENGTH );
    this->rating_type = r.rating_type;
    this->top_count = r.top_count;
    this->start_date = r.start_date;
    this->finish_date = r.finish_date;
    this->is_on_first = r.is_on_first;
    this->is_adult = r.is_adult;
    this->gender = r.gender;
    this->is_deleted = r.is_deleted;
    this->priority = r.priority;

    return *this;
  }

  bool
  operator<( Rating const & r ) const
  {
    return this->id < r.id;
  }

// members

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

}; // Rating


typedef vset::vtree::vtree< vset::vtree::aspect<Rating, std::less<Rating>, VSET_VECTOR_LENGTH> > rating_tree_t;


} // ns rate



int main()
{
  std::string preffix = ".";  

  rate::rating_tree_t ratings;

  ratings.get_allocator().memory().buffer().open( (preffix + "/ratings.bin").c_str() );
 
   

  return 0;
}


