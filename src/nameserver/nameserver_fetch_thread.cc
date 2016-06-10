/*
 * src/nameserver/.cc
 *
 * Copyright (C) 2016 Michael(311155@qq.com). All rights reserved.
 */

#include "nameserver_fetch_thread.h"

namespace sb {
namespace nameserver {
const int64_t ObRootFetchThread::WAIT_INTERVAL = 100 * 1000; // 100 ms

ObRootFetchThread::ObRootFetchThread() : recover_ret_(common::OB_SUCCESS), is_recover_done_(false), log_manager_(NULL) {
}

int ObRootFetchThread::wait_recover_done() {
  int count = 0;
  while (!is_recover_done_) {
    count++;
    if ((count % 100) == 0) {
      TBSYS_LOG(INFO, "wait recover from check point used %d seconds", (count / 10));
    }
    usleep(WAIT_INTERVAL);
  }

  return recover_ret_;
}

int ObRootFetchThread::got_ckpt(uint64_t ckpt_id) {

  TBSYS_LOG(DEBUG, "fetch got checkpoint %lu", ckpt_id);

  if (log_manager_ == NULL) {
    recover_ret_ = common::OB_INVALID_ARGUMENT;
    TBSYS_LOG(ERROR, "log manager is not set");
  } else {
    recover_ret_ = log_manager_->recover_checkpoint(ckpt_id);
  }

  is_recover_done_ = true;

  TBSYS_LOG(INFO, "recover from checkpoint %lu return %d", ckpt_id, recover_ret_);

  return recover_ret_;
}

void ObRootFetchThread::set_log_manager(NameServerLogManager* log_manager) {
  log_manager_ = log_manager;
}

} /* nameserver */
} /* sb */

