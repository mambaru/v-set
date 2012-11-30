//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef VSET_VSET_BUFFER_PERSISTENT_TAGS_HPP
#define VSET_VSET_BUFFER_PERSISTENT_TAGS_HPP

namespace vset { namespace buffer{ namespace persistent{

struct _open_;
struct _before_open_;
struct _after_open_;
struct _close_;
struct _before_close_;
struct _after_close_;
struct _sync_;
struct _before_sync_;
struct _after_sync_;

struct _head_type_;

struct _head_;
struct _size_value_;
struct _capacity_value_;
struct _file_status_;

struct _buffer_;
struct _descriptor_;
struct _file_name_;
struct _open_file_;
struct _close_file_;
struct _file_size_;

struct _seek_set_;
struct _read_file_;
struct _write_file_;

}}}

#endif
