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
#define START_MARK 2000.0

// max index std::numeric_limits<uint32_t>::max(), если индексов будет больше, то использовать size_t
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


//typedef vset::vtree::vtree< vset::vtree::aspect<Rating, std::less<Rating>, VSET_VECTOR_LENGTH> > rating_tree_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~


typedef id_t     content_id_t;
typedef id_t     owner_id_t;
typedef id_t     country_id_t;
typedef uint16_t age_id_t;
typedef uint16_t loc_id_t;


struct Content
{
  Content()
    : id(0),
      owner_id(0),
      rating_id(0),
      country_id(0),
      age_id(0),
      loc1_id(0),
      loc2_id(0),
      loc3_id(0),
      is_intim(false),
      is_real(false),
      mark(START_MARK),
      updated(0),
      is_deleted(false)
  {
  }

  /*
  bool
  operator< ( Content const & c ) const
  {
    return this->id < c.id;
  }
  */

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
  bool          is_deleted; // создаем индекс в дереве удаленных контентов, и помечаем флаг (для GetExpiredContent и etc.)

}; // Content


typedef vset::memory::manager< vset::memory::fsb::aspect<Content> > content_storage_t;


struct ContentByIdFunctor
{
  typedef typename content_storage_t::pointer   content_pointer_t;
  //typedef typename content_storage_t::const_pointer   content_const_pointer_t;


  ContentByIdFunctor()
    : leftp ( NULL ), // нельзя использовать, если не инициализированно итератором от хранилища для индексов
      rightp( NULL )
  {
  }
  
  ContentByIdFunctor( content_pointer_t cont_ptr )
    : leftp ( cont_ptr ),
      rightp( cont_ptr )
  {
  }

  bool
  operator()( rate::offset_t lv, rate::offset_t rv ) const
  {
    leftp  = static_cast<size_t>(lv);
    rightp = static_cast<size_t>(rv); // call's operator size_t()
    return leftp->id < rightp->id;
  }
 
  content_pointer_t mutable leftp;
  content_pointer_t mutable rightp;

}; //ContentByIdFunctor

typedef vset::vtree::vtree< vset::vtree::aspect<rate::offset_t, ContentByIdFunctor> > content_by_rating_index_t;


} // ns rate


int main()
{
  size_t const items = 1000000;
  std::string preffix = "."; 

  rate::content_storage_t          contents; 
  rate::content_by_rating_index_t  contents_by_ratings_i( rate::ContentByIdFunctor( contents.end() ) );

  contents.buffer().open( (preffix + "/contents.bin").c_str() );
  contents.buffer().reserve( items * sizeof(rate::Content) );
  
  contents_by_ratings_i.get_allocator().memory().buffer().open( (preffix + "/contents_by_ratings_i.bin").c_str() );
  contents_by_ratings_i.get_allocator().memory().buffer().reserve( items * sizeof(rate::offset_t) );
  
  for ( size_t i = 0; i < items; ++i )
  {
    rate::content_storage_t::pointer ptr = contents.allocate(1);
    
    *ptr = rate::Content();
    //new(ptr) rate::Content();
    ptr->id = i;
    ptr->owner_id = i + 1;
    ptr->rating_id = i + 2;

    rate::content_by_rating_index_t::iterator it = contents_by_ratings_i.find( static_cast<size_t>(ptr) );   

    if ( it == contents_by_ratings_i.end() )
    {
      contents_by_ratings_i.insert( static_cast<size_t>(ptr) );
    }
    else
      contents.deallocate( ptr, 1 );
  }

  rate::content_storage_t::pointer ptr = contents.allocate(1);
  
  *ptr = rate::Content();
  //new(ptr) rate::Content();
  ptr->id = 133;

  rate::content_by_rating_index_t::iterator it = contents_by_ratings_i.find( static_cast<size_t>(ptr) ); 

  if ( it != contents_by_ratings_i.end() )
  {
    size_t offset = *it;
    std::cout << offset << std::endl;
    contents.deallocate( ptr, 1 );  
    ptr = static_cast<size_t>(*it);
    std::cerr << ptr->owner_id << ", " << ptr->rating_id << "\n";
  }
  else
  {
    contents.deallocate( ptr, 1 );
    std::cerr << "fail\n";
  }

  
  

  return 0;
}



/*
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
*/




