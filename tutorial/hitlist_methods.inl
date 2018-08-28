  void set_hit(uint32_t src, uint32_t dst, time_t ts) 
  {
    _hitlist.set_hit(src, dst, ts);
  }

  void get_hits( std::vector<hit>& hits, uint32_t dst_id, size_t offset, size_t limit) const
  {
    hits.clear();
    hits.reserve(limit);
    _hitlist.get_hits(hits, dst_id, offset, limit);
  }

  size_t delete_user(uint32_t id)
  {
    return _hitlist.delete_user(id);
  }

  size_t remove_outdated(time_t ts)
  {
    return _hitlist.remove_outdated(ts);
  }

  size_t size() const
  {
    return _hitlist.size();
  }
  
  // сколько просмотрел пользователь с идентификатором id
  size_t src_count(uint32_t id) const
  {
    return _hitlist.src_count(id);
  }
  
  // сколько было просмотров у пользователя с идентификатором id
  size_t dst_count(uint32_t id) const
  {
    return _hitlist.dst_count(id);
  }
  
  // сколько просмотров было позже ts
  size_t outdated_count(time_t ts) const
  {
    return _hitlist.outdated_count(ts);
  }
  
  void for_each(std::function<void(hit h)> handler)
  {
    _hitlist.for_each(handler);
  }
