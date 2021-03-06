/**
 * (C) 2010-2011 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Version: $Id$
 *
 * mock_update_server2.cc for ...
 *
 * Authors:
 *   rongxuan <rongxuan.lc@taobao.com>
 *
 */
#include "mock_define.h"
#include "mock_update_server2.h"
#include "common/ob_result.h"
#include "common/ob_define.h"
#include "common/ob_scanner.h"
#include "common/ob_tablet_info.h"

using namespace sb::common;
using namespace sb::updateserver;
using namespace sb::updateserver::test;

int MockUpdateServer::initialize() {
  set_listen_port(UPDATE_SERVER_PORT);
  return MockServer::initialize();
}

ObPacket* MockUpdateServer::do_request(ObPacket* base_packet, bool& is_done) {
  int ret = OB_SUCCESS;
  ObPacket* ob_packet = base_packet;
  int32_t packet_code = ob_packet->get_packet_code();
  ret = ob_packet->deserialize();
  if (OB_SUCCESS == ret) {
    switch (packet_code) {
    case OB_GET_REQUEST: {
#if false
      ret = handle_mock_get(ob_packet);
#else
      ret = handle_get_table(ob_packet);
#endif
      break;
    }
    case OB_SCAN_REQUEST: {
#if false
      ret = handle_mock_scan(ob_packet);
#else
      ret = handle_scan_table(ob_packet);
#endif
      break;
    }
    default: {
      ret = OB_ERROR;
      TBSYS_LOG(ERROR, "wrong packet code:packet[%d]", packet_code);
      break;
    }
    }
    //
    if (ret != OB_SUCCESS) {
      TBSYS_LOG(ERROR, "check handle failed:ret[%d]", ret);
    }
  }
  return NULL;
}


int MockUpdateServer::handle_mock_scan(ObPacket* ob_packet) {
  int ret = OB_SUCCESS;
  ObDataBuffer* data = ob_packet->get_buffer();
  if (NULL == data) {
    ret = OB_ERROR;
  }

  ObScanParam scan_param;
  if (OB_SUCCESS == ret) {
    ret = scan_param.deserialize(data->get_data(), data->get_capacity(), data->get_position());
    if (ret != OB_SUCCESS) {
      TBSYS_LOG(ERROR, "%s", "check scan_param failed");
    }
  }

  tbnet::Connection* connection = ob_packet->get_connection();
  ThreadSpecificBuffer::Buffer* thread_buffer = response_packet_buffer_.get_buffer();
  if (NULL == thread_buffer) {
    ret = OB_ERROR;
  } else {
    thread_buffer->reset();
    ObDataBuffer out_buffer(thread_buffer->current(), thread_buffer->remain());

    ObResultCode result_msg;
    result_msg.result_code_ = ret;
    ret = result_msg.serialize(out_buffer.get_data(), out_buffer.get_capacity(), out_buffer.get_position());

    // fake cell
    ObCellInfo cell;
    ObScanner scanner;
    ObString row_key;
    ObString column_name;
    cell.table_id_ = scan_param.get_table_id();
    if (mock::join_table_id == cell.table_id_) {
      for (int32_t i = 0; i < scan_param.get_column_id_size(); i++) {
        if (mock::join_column1_id == scan_param.get_column_id()[i]) {
          row_key.assign((char*)mock::join_rowkey, strlen(mock::join_rowkey));
          cell.column_id_ = mock::join_column1_id;
          cell.row_key_ = row_key;
          cell.value_.set_int(mock::join_column1_ups_value_1, true);
          ret = scanner.add_cell(cell);
          if (OB_SUCCESS == ret) {
            cell.value_.set_int(mock::join_column1_ups_value_2, true);
            ret = scanner.add_cell(cell);
          }
        } else if (mock::join_column2_id == scan_param.get_column_id()[i]) {
          row_key.assign((char*)mock::join_rowkey, strlen(mock::join_rowkey));
          cell.column_id_ = mock::join_column2_id;
          cell.row_key_ = row_key;
          cell.value_.set_int(mock::join_column2_ups_value_1, true);
          ret = scanner.add_cell(cell);
          if (OB_SUCCESS == ret) {
            cell.value_.set_int(mock::join_column2_ups_value_2, true);
            ret = scanner.add_cell(cell);
          }
        } else {
          TBSYS_LOG(ERROR, "unepxected column id [tableid:%lu,columnid:%lu]", cell.table_id_,
                    scan_param.get_column_id()[i]);
          ret = OB_ERR_UNEXPECTED;
        }
      }
    } else if (mock::table_id == cell.table_id_) {
      for (int32_t i = 0; i < scan_param.get_column_id_size(); i++) {
        if (mock::column1_id == scan_param.get_column_id()[i]) {
          row_key.assign((char*)mock::rowkey, strlen(mock::rowkey));
          cell.column_id_ = mock::column1_id;
          cell.row_key_ = row_key;
          cell.value_.set_int(mock::column1_ups_value_1, true);
          ret = scanner.add_cell(cell);
          if (OB_SUCCESS == ret) {
            cell.value_.set_int(mock::column1_ups_value_2, true);
            ret = scanner.add_cell(cell);
          }
        } else if (mock::column2_id == scan_param.get_column_id()[i]) {
          row_key.assign((char*)mock::rowkey, strlen(mock::rowkey));
          cell.column_id_ = mock::column2_id;
          cell.row_key_ = row_key;
          cell.value_.set_int(mock::column2_ups_value_1, true);
          ret = scanner.add_cell(cell);
          if (OB_SUCCESS == ret) {
            cell.value_.set_int(mock::column2_ups_value_2, true);
            ret = scanner.add_cell(cell);
          }
        } else {
          TBSYS_LOG(ERROR, "unepxected column id [tableid:%lu,columnid:%lu]", cell.table_id_,
                    scan_param.get_column_id()[i]);
          ret = OB_ERR_UNEXPECTED;
        }
      }
    } else {
      TBSYS_LOG(ERROR, "unexpected table id [tableid:%lu]", cell.table_id_);
      ret = OB_ERR_UNEXPECTED;
    }

    if (OB_SUCCESS == ret) {
      ret = scanner.set_is_req_fullfilled(true, 1);
      // scanner.set_timestamp(mock::schema_timestamp);
    }
    int64_t pos = 0;
    char range_buf[512];
    ObString range_str;
    if (OB_SUCCESS == ret) {
      ObRange range;
      range.border_flag_.set_min_value();
      range.border_flag_.set_max_value();
      ret = range.serialize(range_buf, sizeof(range_buf), pos);
      if (OB_SUCCESS == ret) {
        range_str.assign(range_buf, pos);
        // ret = scanner.set_ext_info(range_str);
      }
      pos = 0;
      TBSYS_LOG(INFO, "pos:%ld,ret:%d", pos,
                range.deserialize(range_str.ptr(), range_str.length(), pos));
    }
    int32_t channel_id = ob_packet->getChannelId();
    if (OB_SUCCESS == ret) {
      ret = scanner.serialize(out_buffer.get_data(), out_buffer.get_capacity(), out_buffer.get_position());
    }
    if (OB_SUCCESS == ret) {
      ret = send_response(OB_GET_RESPONSE, 1, out_buffer, connection, channel_id);
    }
  }
  TBSYS_LOG(INFO, "handle scan root table result:ret[%d]", ret);
  return ret;
}

int MockUpdateServer::handle_mock_get(ObPacket* ob_packet) {
  int ret = OB_SUCCESS;
  ObDataBuffer* data = ob_packet->get_buffer();
  if (NULL == data) {
    ret = OB_ERROR;
  }

  ObGetParam get_param;
  if (OB_SUCCESS == ret) {
    ret = get_param.deserialize(data->get_data(), data->get_capacity(), data->get_position());
    if (ret != OB_SUCCESS) {
      TBSYS_LOG(ERROR, "%s", "check param failed");
    }
  }

  tbnet::Connection* connection = ob_packet->get_connection();
  ThreadSpecificBuffer::Buffer* thread_buffer = response_packet_buffer_.get_buffer();
  if (NULL == thread_buffer) {
    ret = OB_ERROR;
  } else {
    thread_buffer->reset();
    ObDataBuffer out_buffer(thread_buffer->current(), thread_buffer->remain());

    ObResultCode result_msg;
    result_msg.result_code_ = ret;
    ret = result_msg.serialize(out_buffer.get_data(), out_buffer.get_capacity(), out_buffer.get_position());

    // fake cell
    ObCellInfo cell;
    ObScanner scanner;
    ObString row_key;
    ObString column_name;
    for (int32_t i = 0; i < get_param.get_cell_size(); i ++) {
      cell.table_id_ = get_param[i]->table_id_;
      cell.column_id_ = get_param[i]->column_id_;
      if (mock::join_table_id == cell.table_id_) {
        if (mock::join_column1_id == cell.column_id_) {
          row_key.assign((char*)mock::join_rowkey, strlen(mock::join_rowkey));
          cell.column_id_ = mock::join_column1_id;
          cell.row_key_ = row_key;
          cell.value_.set_int(mock::join_column1_ups_value_1, true);
          ret = scanner.add_cell(cell);
          if (OB_SUCCESS == ret) {
            cell.value_.set_int(mock::join_column1_ups_value_2, true);
            ret = scanner.add_cell(cell);
          }
        } else if (mock::join_column2_id == cell.column_id_) {
          row_key.assign((char*)mock::join_rowkey, strlen(mock::join_rowkey));
          cell.column_id_ = mock::join_column2_id;
          cell.row_key_ = row_key;
          cell.value_.set_int(mock::join_column2_ups_value_1, true);
          ret = scanner.add_cell(cell);
          if (OB_SUCCESS == ret) {
            cell.value_.set_int(mock::join_column2_ups_value_2, true);
            ret = scanner.add_cell(cell);
          }
        } else if (0 == cell.column_id_) {
          row_key.assign((char*)mock::join_rowkey, strlen(mock::join_rowkey));
          cell.column_id_ = mock::join_column1_id;
          cell.row_key_ = row_key;
          cell.value_.set_int(mock::join_column1_ups_value_1, true);
          ret = scanner.add_cell(cell);
          if (OB_SUCCESS == ret) {
            cell.value_.set_int(mock::join_column1_ups_value_2, true);
            ret = scanner.add_cell(cell);
          }
          if (OB_SUCCESS == ret) {
            row_key.assign((char*)mock::join_rowkey, strlen(mock::join_rowkey));
            cell.column_id_ = mock::join_column2_id;
            cell.row_key_ = row_key;
            cell.value_.set_int(mock::join_column2_ups_value_1, true);
            ret = scanner.add_cell(cell);
            if (OB_SUCCESS == ret) {
              cell.value_.set_int(mock::join_column2_ups_value_2, true);
              ret = scanner.add_cell(cell);
            }
          }
          cell.column_id_ = 0;
        } else {
          TBSYS_LOG(ERROR, "unepxected column id [tableid:%lu,columnid:%lu]", cell.table_id_,
                    cell.column_id_);
          ret = OB_ERR_UNEXPECTED;
        }
      } else if (mock::table_id == cell.table_id_) {
        if (mock::column1_id == cell.column_id_) {
          row_key.assign((char*)mock::rowkey, strlen(mock::rowkey));
          cell.column_id_ = mock::column1_id;
          cell.row_key_ = row_key;
          cell.value_.set_int(mock::column1_ups_value_1, true);
          ret = scanner.add_cell(cell);
          if (OB_SUCCESS == ret) {
            cell.value_.set_int(mock::column1_ups_value_2, true);
            ret = scanner.add_cell(cell);
          }
        } else if (mock::column2_id == cell.column_id_) {
          row_key.assign((char*)mock::rowkey, strlen(mock::rowkey));
          cell.column_id_ = mock::column2_id;
          cell.row_key_ = row_key;
          cell.value_.set_int(mock::column2_ups_value_1, true);
          ret = scanner.add_cell(cell);
          if (OB_SUCCESS == ret) {
            cell.value_.set_int(mock::column2_ups_value_2, true);
            ret = scanner.add_cell(cell);
          }
        } else {
          TBSYS_LOG(ERROR, "unepxected column id [tableid:%lu,columnid:%lu]", cell.table_id_,
                    cell.column_id_);
          ret = OB_ERR_UNEXPECTED;
        }
      } else {
        TBSYS_LOG(ERROR, "unexpected table id [tableid:%lu]", cell.table_id_);
        ret = OB_ERR_UNEXPECTED;
      }
    }


    if (OB_SUCCESS == ret) {
      int64_t fullfilled_item_num = 0;
      if (0 == cell.column_id_) {
        fullfilled_item_num = 1;
      } else {
        fullfilled_item_num = 2;
      }
      ret = scanner.set_is_req_fullfilled(true, fullfilled_item_num);
      //scanner.set_timestamp(mock::schema_timestamp);
    }
    int32_t channel_id = ob_packet->getChannelId();
    ret = scanner.serialize(out_buffer.get_data(), out_buffer.get_capacity(), out_buffer.get_position());
    ret = send_response(OB_GET_RESPONSE, 1, out_buffer, connection, channel_id);
  }
  TBSYS_LOG(INFO, "handle scan root table result:ret[%d]", ret);
  return ret;
}



int MockUpdateServer::handle_scan_table(ObPacket* ob_packet) {
  int ret = OB_SUCCESS;
  ObDataBuffer* data = ob_packet->get_buffer();
  if (NULL == data) {
    ret = OB_ERROR;
  }

  ObScanParam param;
  if (OB_SUCCESS == ret) {
    ret = param.deserialize(data->get_data(), data->get_capacity(), data->get_position());
    if (ret != OB_SUCCESS) {
      TBSYS_LOG(ERROR, "%s", "check param failed");
    }
  }

  tbnet::Connection* connection = ob_packet->get_connection();
  ThreadSpecificBuffer::Buffer* thread_buffer = response_packet_buffer_.get_buffer();
  if (NULL == thread_buffer) {
    ret = OB_ERROR;
  } else {
    thread_buffer->reset();
    ObDataBuffer out_buffer(thread_buffer->current(), thread_buffer->remain());

    ObResultCode result_msg;
    result_msg.result_code_ = ret;
    ret = result_msg.serialize(out_buffer.get_data(), out_buffer.get_capacity(), out_buffer.get_position());

    // fake cell
    ObCellInfo cell;
    ObScanner scanner;
    ObString row_key;
    ObString column_name;
    char temp[256] = "";
    ObString table_name;
    char* name = "update_test_table";
    table_name.assign(name, strlen(name));
    cell.table_name_ = table_name;
    for (uint64_t i = 0; i < 10; ++i) {
      snprintf(temp, 256, "update_%lu_scan_row_key:%lu", i, i);
      row_key.assign(temp, strlen(temp));
      cell.row_key_ = row_key;
      column_name.assign(temp, strlen(temp));
      cell.column_name_ = column_name;
      cell.value_.set_int(2234 + i);
      scanner.add_cell(cell);
    }

    int32_t channel_id = ob_packet->getChannelId();
    ret = scanner.serialize(out_buffer.get_data(), out_buffer.get_capacity(), out_buffer.get_position());
    //
    ret = send_response(OB_GET_RESPONSE, 1, out_buffer, connection, channel_id);
  }
  TBSYS_LOG(INFO, "handle scan root table result:ret[%d]", ret);
  return ret;
}

int MockUpdateServer::handle_get_table(ObPacket* ob_packet) {
  int ret = OB_SUCCESS;
  ObDataBuffer* data = ob_packet->get_buffer();
  if (NULL == data) {
    ret = OB_ERROR;
  }

  ObGetParam param;
  if (OB_SUCCESS == ret) {
    ret = param.deserialize(data->get_data(), data->get_capacity(), data->get_position());
    if (ret != OB_SUCCESS) {
      TBSYS_LOG(ERROR, "%s", "check param failed");
    }
  }

  tbnet::Connection* connection = ob_packet->get_connection();
  ThreadSpecificBuffer::Buffer* thread_buffer = response_packet_buffer_.get_buffer();
  if (NULL == thread_buffer) {
    ret = OB_ERROR;
  } else {
    thread_buffer->reset();
    ObDataBuffer out_buffer(thread_buffer->current(), thread_buffer->remain());

    ObResultCode result_msg;
    result_msg.result_code_ = ret;
    ret = result_msg.serialize(out_buffer.get_data(), out_buffer.get_capacity(), out_buffer.get_position());

    // fake data cell
    ObCellInfo cell;
    ObScanner scanner;
    ObString row_key;
    ObString column_name;
    char temp[256] = "";
    ObString table_name;
    char* name = "update_test_table";
    table_name.assign(name, strlen(name));
    cell.table_name_ = table_name;
    for (uint64_t i = 0; i < 10; ++i) {
      snprintf(temp, 256, "update_%lu_get_row_key:%lu", i, i);
      row_key.assign(temp, strlen(temp));
      cell.row_key_ = row_key;
      column_name.assign(temp, strlen(temp));
      cell.column_name_ = column_name;
      cell.value_.set_int(2234 + i);
      scanner.add_cell(cell);
    }

    int32_t channel_id = ob_packet->getChannelId();
    ret = scanner.serialize(out_buffer.get_data(), out_buffer.get_capacity(), out_buffer.get_position());
    //
    ret = send_response(OB_SCAN_RESPONSE, 1, out_buffer, connection, channel_id);
  }
  TBSYS_LOG(INFO, "handle scan root table result:ret[%d]", ret);
  return ret;
}






