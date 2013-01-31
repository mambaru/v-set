#pragma once

#include <string>

typedef unsigned int id_t;
typedef unsigned int offset_t;

/**
 * Контейнеры vset написаны с использованием faslib2 которая не совместима с faslib
 * Пожтому логику работы выделить в отдельную библиотеку
 */


/// Хранилище пар за какой контент проголосовал пользователь
class poller_storage;

/// Неупорядочное хранилище данных о контенте (фотках)
class content_storage;
/// Индекс по идентификатору контента
class content_index;
/// Индекс по идентификатору рейтинга и контента
class content_by_rating_index;
/// Индекс по идентификатору пользователя и контента
/// Индекс по рейтигу контента
/// Индекс по рейтигу контента и стране

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
  //vote_storage* _votes;
  content_storage* _content;
  //rating_poller_content_index* _rating_poller_content_index;
  content_index* _content_index;
  content_by_rating_index* _content_by_rating_index;
};