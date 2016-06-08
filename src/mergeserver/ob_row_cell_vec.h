/**
 * (C) 2010-2011 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Version: $Id$
 *
 * ob_row_cell_vec.h for ...
 *
 * Authors:
 *   wushi <wushi.ly@taobao.com>
 *
 */
#ifndef OCEANBASE_MERGESERVER_OB_ROW_CELL_VEC_H_
#define OCEANBASE_MERGESERVER_OB_ROW_CELL_VEC_H_
#include "common/ob_common_param.h"
#include "common/ob_iterator.h"
#include "common/ob_string.h"
#include "common/ob_schema.h"
namespace sb {
namespace mergeserver {
class ObRowCellVec : public sb::common::ObIterator {
 public:
  ObRowCellVec();
  virtual ~ObRowCellVec();

  int init();

  bool is_empty();

  int add_cell(const sb::common::ObCellInfo& cell);

  inline const sb::common::ObString& get_row_key()const {
    return row_key_;
  }

  uint64_t get_table_id()const {
    return table_id_;
  }

  void reset_iterator();
  virtual int next_cell();
  virtual int get_cell(sb::common::ObCellInfo** cell);
  virtual int get_cell(sb::common::ObCellInfo** cell, bool* is_row_changed);

  int serialize(char* buf, const int64_t buf_len, int64_t& pos) const;
  int deserialize(const char* buf, const int64_t data_len, int64_t& pos);
  int64_t get_serialize_size(void) const;

 private:
  struct ObMsCellInfo {
    uint64_t column_id_;
    sb::common::ObObj value_;
  };
  struct ObRow {
    int32_t cell_num_;
    int32_t row_key_len_;
    uint64_t table_id_;
    ObMsCellInfo cells_[0];
  };
  char*    row_buffer_;
  int32_t row_buffer_size_;
  int32_t cur_buffer_offset_;
  ObRow*   row_;
  sb::common::ObString row_key_;
  uint64_t                    table_id_;
  int32_t                     consumed_cell_num_;
  bool                        is_empty_;
  sb::common::ObCellInfo cur_cell_;
};
}
}



#endif /* OCEANBASE_MERGESERVER_OB_ROW_CELL_VEC_H_ */


