//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_FILE_ASPECT_HPP
#define VSET_VSET_BUFFER_PERSISTENT_FILE_ASPECT_HPP

#include <vset/buffer/tags.hpp>
#include <vset/buffer/persistent/tags.hpp>
#include <vset/buffer/persistent/head.hpp>
#include <vset/buffer/persistent/file/tags.hpp>
#include <vset/buffer/persistent/file/ad_head.hpp>
#include <vset/buffer/persistent/file/ad_data.hpp>
#include <vset/buffer/persistent/file/ad_value.hpp>
#include <vset/buffer/persistent/file/ad_close_file.hpp>
#include <vset/buffer/persistent/file/ad_open_file.hpp>
#include <vset/buffer/persistent/file/ad_basic_init.hpp>
#include <vset/buffer/persistent/file/ad_read_file.hpp>
#include <vset/buffer/persistent/file/ad_resize_file.hpp>
#include <vset/buffer/persistent/file/ad_write_file.hpp>
#include <vset/buffer/persistent/file/ad_seek_set.hpp>
#include <vset/buffer/persistent/file/ad_file_size.hpp>

#include <fas/aop.hpp>
#include <string>

namespace vset { namespace buffer{ namespace persistent{ namespace file{

struct aspect: fas::aspect< fas::type_list_n<
  fas::group<_constructor_, _basic_init_>,
  fas::group<_destructor_, _close_file_>,
  fas::type< _data_type_, char*>,
  fas::type<_const_data_type_, const char*>,
  fas::type< _size_type_, size_t>,
  fas::type< _head_type_, head<0> >,

  fas::value< _buffer_, char*>,
  fas::value< _size_value_, size_t>,
  fas::value< _capacity_value_, size_t>,
  fas::value< _descriptor_, int>,
  fas::value< _file_status_, bool>,
  fas::value< _file_name_, std::string>,

  fas::advice< _head_, ad_head>,
  fas::advice< _data_, ad_data>,
  fas::advice< _capacity_, ad_value<_size_type_, _capacity_value_> >,
  fas::advice< _size_, ad_value<_size_type_, _size_value_> >,


  fas::advice< _close_file_, ad_close_file>,
  fas::advice< _open_file_, ad_open_file>,
  fas::advice< _read_file_, ad_read_file>,
  fas::advice< _write_file_, ad_write_file>,
  fas::advice< _resize_file_, ad_resize_file>,
  fas::advice< _seek_set_, ad_seek_set>,
  fas::advice< _file_size_, ad_file_size>,
  fas::advice< _basic_init_, ad_basic_init>
>::type >{};

}}}}

#endif

