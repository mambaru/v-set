#pragma once

#include <string>

typedef unsigned int id_t;
typedef unsigned int offset_t;


class vote_storage;
class poller_storage;
class content_storage;
class rating_poller_content_index;
class content_index;
class content_by_rating_index;

class votes
{
public:
  
  votes();
  virtual ~votes();

  void open(const std::string& preffix);
  void close();
  void sync();

  bool add_content(id_t rating_id, id_t content_id, id_t country_id = 0, id_t region_id  = 0, id_t city_id  = 0, id_t metro_id = 0);
  bool add_vote( id_t poller_id, id_t nice_content_id, id_t ugly_content_id );
  
  
private:
  poller_storage* _pollers;
  vote_storage* _votes;
  content_storage* _content;
  rating_poller_content_index* _rating_poller_content_index;
  content_index* _content_index;
  content_by_rating_index* _content_by_rating_index;
};