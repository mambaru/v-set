
hitlist::hitlist()
{
  _impl = new impl;
}
  
hitlist::~hitlist()
{
  delete _impl;
}

void hitlist::open()
{
  _impl->open();
}
  
void hitlist::set_hit(uint32_t src, uint32_t dst, time_t ts) 
{
  _impl->set_hit(src, dst, ts);
}
  
size_t hitlist::get_hits( std::vector<hit>& hits, uint32_t id, size_t offset, size_t limit) const
{
  return _impl->get_hits(hits, id, offset, limit);
}
  
size_t hitlist::size() const
{
  return _impl->size();
}

size_t hitlist::capacity() const
{
  return _impl->capacity();
}

