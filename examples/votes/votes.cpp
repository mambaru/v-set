#include "votes.hpp"
#include <vset/vtree/vtree.hpp>
#include <vset/memory/manager.hpp>
#include <iostream>


struct vote
{
  id_t rating_id;
  id_t poller_id;
  id_t content_id;
  double mark;

  vote()
    : rating_id(0)
    , poller_id(0)
    , content_id(0)
    , mark(0.0)
  {}
};

struct content
{
  id_t rating_id;
  id_t content_id;
  
  id_t country_id;
  id_t region_id;
  id_t city_id;
  id_t metro_id;
  
  int votes;
  double mark;

  content()
    : rating_id(0)
    , content_id(0)
    , country_id(0)
    , region_id(0)
    , city_id(0)
    , metro_id(0)
    , votes(0)
    , mark(0)
  {}

  content(id_t content_id)
    : rating_id(0)
    , content_id(content_id)
    , country_id(0)
    , region_id(0)
    , city_id(0)
    , metro_id(0)
    , votes(0)
    , mark(0)
  {}

  
  content(id_t rating_id, id_t content_id, double mark = 2000.0, id_t country_id = 0, id_t region_id  = 0, id_t city_id  = 0, id_t metro_id = 0)
    : rating_id(rating_id)
    , content_id(content_id)
    , country_id(country_id)
    , region_id(region_id)
    , city_id(city_id)
    , metro_id(metro_id)
    , votes(0)
    , mark(mark)
  {}

  /*bool operator < (const content& right) const
  {
    return content_id < right.content_id;
  }
  */
};

class content_storage
  : public vset::memory::manager< vset::memory::fsb::aspect<content> >
  //: public vset::vtree::vtree< vset::vtree::aspect<content> >
{};

typedef content_storage::pointer content_pointer;
typedef content_storage::const_pointer const_content_pointer;

struct f_content_by_id
{
  mutable content_pointer pleft;
  mutable content_pointer pright;

  f_content_by_id()
    : pleft(0)
    , pright(0)
  {}

  f_content_by_id(content_pointer ptr)
    : pleft(ptr)
    , pright(ptr)
  {
  }

  bool operator()(offset_t left, offset_t right) const
  {
    pleft = left;
    pright = right;

    std::cout << "f_content_by_id: " << pleft->content_id << " < " <<  pright->content_id << std::endl;
    return pleft->content_id < pright->content_id;
  }

  bool operator()(offset_t left, const content& right) const
  {
    pleft = left;
    return pleft->content_id < right.content_id;
  }

};

class content_index
  : public vset::vtree::vtree< vset::vtree::aspect<offset_t, f_content_by_id> >
{
public:
  content_index(const f_content_by_id& comp)
    : vtree(comp)
  {

  }

};


struct f_content_by_rating
{
  mutable content_pointer pleft;
  mutable content_pointer pright;

  f_content_by_rating()
    : pleft(0)
    , pright(0)
  {}

  
  f_content_by_rating(content_pointer ptr)
    : pleft(ptr)
    , pright(ptr)
  {
  }
  
  bool operator()(offset_t left, offset_t right) const
  {
    pleft = static_cast<size_t>(left);
    pright = static_cast<size_t>(right);

    if ( pleft->rating_id == pright->rating_id )
      return pleft->content_id < pright->content_id;
    return pleft->rating_id < pright->rating_id;
  }

  bool operator()(offset_t left, const content& right) const
  {
    pleft = left;
    if ( pleft->rating_id == right.rating_id )
      return pleft->content_id < right.content_id;
    return pleft->rating_id < right.rating_id;
  }

};

class content_by_rating_index
  : public vset::vtree::vtree< vset::vtree::aspect<offset_t, f_content_by_rating> >
{
public:
  content_by_rating_index(const f_content_by_rating& comp)
    : vtree(comp)
  {
  }
};

struct poller
{
  id_t poller_id;
  id_t content_id;

  poller()
    : poller_id(0)
    , content_id(0)
  {}

  poller(id_t poller_id, id_t content_id )
    : poller_id(poller_id)
    , content_id(content_id)
  {
  }
  
  bool operator < (const poller& right) const
  {
    if ( poller_id == right.poller_id )
      return content_id == right.content_id;
    return poller_id < right.poller_id;
  }
};

class vote_storage
 : public vset::memory::manager< vset::memory::fsb::aspect<vote> >
{
};

typedef vote_storage::pointer vote_pointer;

struct f_rating_poller_content
{
  vote_pointer ptr;

  f_rating_poller_content(): ptr(0){}
  
  f_rating_poller_content(vote_pointer ptr): ptr(ptr){}

  bool operator()(vote_pointer left, vote_pointer right) const
  {
    return left->rating_id < right->rating_id;
  }
};


class poller_storage
  : public vset::vtree::vtree< vset::vtree::aspect<poller, std::less<poller>, 512> >
{};

class rating_poller_content_index
  : public vset::vtree::vtree< vset::vtree::aspect<offset_t, f_rating_poller_content, 512> >
{
public:
  rating_poller_content_index(const f_rating_poller_content& comp)
    : vtree(comp)
  {
    
  }
};

// 
class ids_storage: public vset::vtree::vtree< vset::vtree::aspect< id_t> >{};


votes::votes()
{
  _votes = new vote_storage;
  _pollers = new poller_storage;
  _content = new content_storage;
  _content_index= new content_index(f_content_by_id(_content->end()));
  _content_by_rating_index = new content_by_rating_index(f_content_by_rating(_content->end()));
  _rating_poller_content_index = new rating_poller_content_index(f_rating_poller_content(_votes->end()));
}

votes::~votes()
{
  this->sync();
  this->close();
  
  delete _votes;
  delete _pollers;
  delete _content;
  delete _content_index;
  delete _content_by_rating_index;
  delete _rating_poller_content_index;
}

void votes::open(const std::string& preffix)
{
  _votes->buffer().open( (preffix + "/votes.bin").c_str() );
  _pollers->get_allocator().memory().buffer().open( (preffix + "/pollers.bin").c_str() );
  _content->buffer().open( (preffix + "/content.bin").c_str() );
  //_content->get_allocator().memory().buffer().open( (preffix + "/content.bin").c_str() );
  _content_index->get_allocator().memory().buffer().open( (preffix + "/content_by_id.bin").c_str() );
  _content_by_rating_index->get_allocator().memory().buffer().open( (preffix + "/content_by_rating.bin").c_str() );
  _rating_poller_content_index->get_allocator().memory().buffer().open( (preffix + "/votes-index1.bin").c_str() );
}


void votes::sync()
{
  _votes->buffer().sync(false);
  _pollers->get_allocator().memory().buffer().sync(false);
  _content->buffer().sync(false);
  _content_by_rating_index->get_allocator().memory().buffer().sync(false);
  _content_index->get_allocator().memory().buffer().sync(false);
  _rating_poller_content_index->get_allocator().memory().buffer().sync(false);
}

void votes::close()
{
  _votes->buffer().close();
  _pollers->get_allocator().memory().buffer().close();
  _content->buffer().close();
  _content_by_rating_index->get_allocator().memory().buffer().close();
  _content_index->get_allocator().memory().buffer().close();
  _rating_poller_content_index->get_allocator().memory().buffer().close();
}

bool votes::add_content(id_t rating_id, id_t content_id, id_t country_id, id_t region_id, id_t city_id, id_t metro_id)
{
  std::cout << "bool votes::add_content(id_t rating_id, id_t content_id, id_t country_id, id_t region_id, id_t city_id, id_t metro_id)" << std::endl;
  content_pointer ptr = _content->allocate(1);
  *ptr = content(rating_id, content_id, country_id, region_id, city_id, metro_id);
  
  content_index::iterator itr = _content_index->lower_bound( static_cast<size_t>(ptr) );

  if ( itr != _content_index->end() )
  {
    std::cout << *itr << std::endl;
    content_pointer ptr2 = _content->begin() + *itr;
    if ( ptr2->content_id == content_id)
    {
      _content->deallocate(ptr2, 1);
      return false;
    }
  }

  _content_index->insert( static_cast<offset_t>( static_cast<size_t>(ptr) ) );
  _content_by_rating_index->insert( static_cast<offset_t>( static_cast<size_t>(ptr) ) );
  return true;
}

bool votes::add_vote(id_t poller_id, id_t nice_content_id, id_t ugly_content_id )
{
  if (_pollers->find( poller(poller_id, nice_content_id) ) != _pollers->end() )
    return false;

  if (_pollers->find( poller(poller_id, ugly_content_id) ) != _pollers->end() )
    return false;

  content_pointer ptr = _content->allocate(1);
  
  *ptr = content(nice_content_id);
  content_index::iterator itr1 = _content_index->find( static_cast<size_t>(ptr) );
  
  *ptr = content(ugly_content_id);
  content_index::iterator itr2 = _content_index->find( static_cast<size_t>(ptr) );

  _content->deallocate(ptr, 1);

  if ( itr1 == _content_index->end() || itr2 == _content_index->end() )
    return false;

  /*
  content_storage::iterator itr1 = _content->find( content(nice_content_id) );
  if ( itr1 == _content->end() )
    return false;

  content_storage::iterator itr2 = _content->find( content(ugly_content_id) );
  if ( itr2 == _content->end() )
    return false;
  */
  

  ptr = *itr1;
  content_pointer ptr2 = _content->end();
  ptr2 = *itr2;

  
  double delta_mark = std::min( ptr->mark, ptr2->mark ) * 0.05;
  
  
  ptr->mark += delta_mark;
  ptr2->mark -= delta_mark;
  
  _pollers->insert( poller(poller_id, nice_content_id) );
  _pollers->insert( poller(poller_id, ugly_content_id) );
  
  return true;
}