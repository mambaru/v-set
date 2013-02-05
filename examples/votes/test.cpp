#include <iostream>
#include <string>
#include <ctime>
#include <cstring>
#include <stdint.h>
#include <algorithm> // std::move
#include <set>
#include <fstream>

#include <chrono>

#include <vset/vtree/vtree.hpp>
#include <vset/memory/manager.hpp>

#include <stx/btree_set.h>
#include <stx/btree_multiset.h>

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
    this->name[0] = 0;
  }

  explicit
  Rating ( rating_id_t rid )
  {
    this->id = rid;
    this->name[0] = 0;
  }

  Rating ( Rating const & r )
  {
    *this = r;
  }

  Rating &
  operator= ( Rating const & r )
  {
    this->id = r.id;
    this->set_name( r.name );
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

  inline
  void
  set_name ( char const * str )
  {
    std::strncpy( this->name, str, NAME_LENGTH );
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


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~


typedef id_t     content_id_t;
typedef id_t     owner_id_t;
typedef id_t     country_id_t;
typedef uint16_t age_id_t;
typedef uint16_t loc_id_t;

struct Content
{
  Content()
  {
    this->id = 0;
    this->rating_id = 0;
  }

  bool
  operator< ( Content const & c ) const
  {
    return this->id < c.id;
  }

//members

  content_id_t  id;
  owner_id_t    owner_id;
  rating_id_t   rating_id;
  country_id_t  country_id;
  age_id_t      age_id;
  loc_id_t      loc1_id; 
  loc_id_t      loc2_id; 
  loc_id_t      loc3_id;
  bool          is_intim;
  bool          is_real;
  double        mark;
  time_t        updated; 

}; // Content


} // ns rate



int main()
{
  size_t const iters = 10000000;

#if 1 
  std::string preffix = ".";  

  rate::rating_tree_t ratings;

  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  
  ratings.get_allocator().memory().buffer().open( (preffix + "/ratings.bin").c_str() );

  ratings.get_allocator().memory().buffer().reserve( iters * sizeof(rate::Rating) * 2 );

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  std::cerr << "Open:\n" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milli sec\n";


#else

  stx::btree_multiset<rate::Rating> ratings;

  //std::multiset<rate::Rating> ratings;  

#endif

  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

  for ( size_t i = 0; i < iters; ++i )
    ratings.insert( rate::Rating( i ) );

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  std::cerr << "Time:\n" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milli sec\n"
                         << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " sec\n";

  std::ofstream f( "btreedump.bin", std::ios::out | std::ios::binary );

  start = std::chrono::steady_clock::now();

  ratings.get_allocator().memory().buffer().sync(false);

  //ratings.dump( f ); 

  end = std::chrono::steady_clock::now();

  std::cerr << "Sync: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milli sec\n";
  //for( int i = 0; i <  )  


  return 0;
}





