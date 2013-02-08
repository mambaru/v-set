#include "votes.hpp"
#include <vset/vtree/vtree.hpp>
#include <vset/memory/manager.hpp>
#include <vset/multiset.hpp>
#include <vset/allocators/mmap_allocator.hpp>
#include <iostream>

/**
 * За какой контент голосовал пользователь
 */
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
    return ( poller_id < right.poller_id )
           || ( !(right.poller_id < poller_id)
           && (content_id < right.content_id) );
  }
};

/// сортированное хранилище по poller_id и content_id
/// 512 - размер сортированного массива в элементах
class poller_storage
  : public vset::vtree::vtree< vset::vtree::aspect<poller, std::less<poller>, 512> >
{};

struct f_poller_by_content
{
  bool operator()(const poller& left, const poller& right) const
  {
    return ( left.content_id < right.content_id ) || ( !(right.content_id < left.content_id) && (left.poller_id < right.poller_id) );
  }
};

/// сортированное хранилище по content_id и poller_id
class poller_by_content
  : public vset::vtree::vtree< vset::vtree::aspect<poller, f_poller_by_content, 512> >
{};


/// Несортированое хранилище
class content_storage
  : public vset::memory::manager< vset::memory::fsb::aspect<content> >
{};

/// Указатель на элемент хранилища, специальный класс-обертка
typedef content_storage::pointer content_pointer;
typedef content_storage::const_pointer const_content_pointer;

/// Компоратор по content_id
struct f_content_by_id
{
  /// В индексах хранится смещение элементов относительно начала буфера
  /// А "указатель" хранит указатель на объект хранилища, чтобы по смещению
  /// получить доступ к элементу
  mutable content_pointer pleft;
  mutable content_pointer pright;

  /// Конструктор по умолчание - указатели не инициализированы (использовать нельзя)
  f_content_by_id()
    : pleft(0)
    , pright(0)
  {}

  /// Инициализируем указатели любым инициализированым, для того чтобы получить ссылку на хранилище
  f_content_by_id(content_pointer ptr)
    : pleft(ptr)
    , pright(ptr)
  {
  }

  bool operator()(offset_t left, offset_t right) const
  {
    /// Задаем сдвиг для указателей
    /// Мы не можем просто создать объект content_pointer, нужно инициализировать его ссылкой на хранилище
    pleft = left;
    pright = right;
    return pleft->content_id < pright->content_id;
  }
};

/// Индекс по content_id
class content_index
  : public vset::vtree::vtree< vset::vtree::aspect<offset_t, f_content_by_id> >
{
public:
  /// Необходимо в индекс передать компоратор с инициализированными указателями
  content_index(const f_content_by_id& comp)
    : vtree(comp)
  {}
};

/// Компоратор по rating_id и content_id
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

    return ( pleft->rating_id < pright->rating_id )
           || ( !(pright->rating_id < pleft->rating_id)
                && (pleft->content_id < pright->content_id) );
  }
};

/// Индекс по rating_id и content_id
class content_by_rating_index
  : public vset::vtree::vtree< vset::vtree::aspect<offset_t, f_content_by_rating> >
{
public:
  content_by_rating_index(const f_content_by_rating& comp)
    : vtree(comp)
  {
  }
};


struct content_by_rating
{
  id_t rating_id;
  id_t content_id;
  offset_t offset;

  content_by_rating(id_t rating_id = 0, id_t content_id = 0, offset_t offset = 0)
    : rating_id(rating_id)
    , content_id(content_id)
    , offset(offset)
  {}

  bool operator < ( const content_by_rating& right) const
  {
    return ( rating_id < right.rating_id )
           || ( !(right.rating_id < rating_id)
                && (content_id < right.content_id) );
  }
};


struct content_by_rating_index2
  : vset::multiset< content_by_rating, std::less<content_by_rating>, vset::mmap_allocator<512> >
{
  
};




votes::votes()
{
  // Создаем сортированное хранилище
  _pollers = new poller_storage;
  // Создаем не сортированное хранилище
  _content = new content_storage;
  // Создаем индексы и явно передаем компаратор с иницализированным указателем
  // указатель можно взять из хранилища, _content->end() - вполне сойдет
  _content_index= new content_index(f_content_by_id(_content->end()));
  _content_by_rating_index = new content_by_rating_index(f_content_by_rating(_content->end()));
  _content_by_rating_index2 = new content_by_rating_index2;
  //_rating_poller_content_index = new rating_poller_content_index(f_rating_poller_content(_votes->end()));
}

votes::~votes()
{
  this->sync();
  this->close();

  delete _pollers;
  delete _content;
  delete _content_index;
  delete _content_by_rating_index;
  delete _content_by_rating_index2;
  //delete _rating_poller_content_index;
}

void votes::open(const std::string& preffix)
{
  // А так открываем файл у vtree
  _pollers->get_allocator().memory().buffer().open( (preffix + "/pollers.bin").c_str() );
  // Так открываем файл у memory::manager
  _content->buffer().open( (preffix + "/content.bin").c_str() );
  _content_index->get_allocator().memory().buffer().open( (preffix + "/content_by_id.bin").c_str() );
  _content_by_rating_index->get_allocator().memory().buffer().open( (preffix + "/content_by_rating.bin").c_str() );
  _content_by_rating_index2->get_allocator().memory().buffer().open( (preffix + "/content_by_rating2.bin").c_str() );

  // если в хранилище есть данные а индекс пустой
  if ( _content->buffer().size()!=0 && _content_index->empty() )
  {
    for ( content_pointer beg = _content->begin(); beg!=_content->end(); ++beg)
      _content_index->insert( static_cast<offset_t>( static_cast<size_t>(beg) ) );
  }
}

void votes::sync()
{
  _pollers->get_allocator().memory().buffer().sync(false);
  _content->buffer().sync(false);
  _content_by_rating_index->get_allocator().memory().buffer().sync(false);
  _content_by_rating_index2->get_allocator().memory().buffer().sync(false);
  _content_index->get_allocator().memory().buffer().sync(false);
}

void votes::close()
{
  _pollers->get_allocator().memory().buffer().close();
  _content->buffer().close();
  _content_by_rating_index->get_allocator().memory().buffer().close();
  _content_by_rating_index2->get_allocator().memory().buffer().close();
  _content_index->get_allocator().memory().buffer().close();
}

bool votes::add_content(id_t rating_id, id_t content_id, id_t country_id, id_t region_id, id_t city_id, id_t metro_id)
{
  // Создаем временный объект для поиска
  // Лучше его сделать членном класса чтобы не создавать каждый раз
  content_pointer ptr = _content->allocate(1);
  *ptr = content(rating_id, content_id, 2000.0, country_id, region_id, city_id, metro_id);

  // Проверяем есть ли контент с таким id
  content_index::iterator itr = _content_index->find( /*преобразовывает в offset_t*/ static_cast<size_t>(ptr) );

  if ( itr != _content_index->end() )
  {
    _content->deallocate(ptr, 1);
    return false;
  }
  
  // Добавляем в индексы
  _content_index->insert( /*static_cast<offset_t>(*/ static_cast<size_t>(ptr) /*)*/ );
  _content_by_rating_index->insert( static_cast<offset_t>( static_cast<size_t>(ptr) ) );
  _content_by_rating_index2->insert( content_by_rating(rating_id, content_id, static_cast<size_t>(ptr) ) );
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

bool votes::get_content_for_rating(id_t rating_id, size_t offset, size_t limit, std::vector<content>& contents) const
{
  typedef content_by_rating_index2::const_iterator const_iterator;
  const_iterator lower = _content_by_rating_index2->lower_bound( content_by_rating(rating_id) );
  const_iterator upper = _content_by_rating_index2->upper_bound( content_by_rating(rating_id, static_cast<id_t>(-1) ) );

  if ( std::distance(lower, upper ) <= offset  )
    return false;

  lower += offset;

  if ( limit < std::distance(lower, upper ) )
  {
    upper = lower + limit;
  }

  content_storage::pointer ptr = _content->end();

  contents.reserve(limit);
  for ( ;lower!=upper; ++lower )
  {
    ptr = lower->offset;
    contents.push_back( *ptr );  
  }

  upper = lower - limit;
  return true;
}

