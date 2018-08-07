
hitlist::hitlist()
{
  _impl = new impl;
}
  
hitlist::~hitlist()
{
  delete _impl;
}

bool hitlist::open(size_t reserve1, size_t reserve2)
{
  return _impl->open(reserve1, reserve2);
}
  
void hitlist::set_hit(uint32_t src, uint32_t dst, time_t ts) 
{
  _impl->set_hit(src, dst, ts);
}
  
void hitlist::get_hits( std::vector<hit>& hits, uint32_t id, size_t offset, size_t limit) const
{
  _impl->get_hits(hits, id, offset, limit);
}

size_t hitlist::remove_outdated(time_t ts)
{
  return _impl->remove_outdated(ts);
}
  
size_t hitlist::size() const
{
  return _impl->size();
}

size_t hitlist::capacity() const
{
  return _impl->capacity();
}

size_t hitlist::delete_user(uint32_t id)
{
  return _impl->delete_user(id);
}

size_t hitlist::src_count(uint32_t id) const
{
  return _impl->src_count(id);
}

size_t hitlist::dst_count(uint32_t id) const
{
  return _impl->dst_count(id);
}
  
size_t hitlist::outdated_count(time_t ts) const
{
  return _impl->outdated_count(ts);
}

void hitlist::for_each(std::function<void(hit h)> handler)
{
  _impl->for_each(handler);
}

std::string hitlist::desc() const
{
  return _impl->desc();
}
