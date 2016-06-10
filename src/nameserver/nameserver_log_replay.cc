/*
 * src/nameserver/.cc
 *
 * Copyright (C) 2016 Michael(311155@qq.com). All rights reserved.
 */

#include "nameserver_log_replay.h"

namespace sb {
namespace nameserver {
ObRootLogReplay::ObRootLogReplay() {
  is_initialized_ = false;
}

ObRootLogReplay::~ObRootLogReplay() {
}

void ObRootLogReplay::set_log_manager(NameServerLogManager* log_manager) {
  log_manager_ = log_manager;
}

int ObRootLogReplay::replay(common::LogCommand cmd, uint64_t seq, const char* log_data, const int64_t data_len) {
  UNUSED(seq);
  int ret = log_manager_->get_log_worker()->apply(cmd, log_data, data_len);
  if (ret != common::OB_SUCCESS) {
    TBSYS_LOG(ERROR, "fatal error, replay log failed, err=[%d]. Quit...", ret);
    exit(120);
    // log_manager_->get_log_worker()->exit();
  }
  return ret;
}
} /* nameserver */
} /* sb */

