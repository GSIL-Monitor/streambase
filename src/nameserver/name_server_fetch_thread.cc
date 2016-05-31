#include "name_server_fetch_thread.h"

namespace sb {
namespace nameserver {
const int64_t NameServerFetchThread::WAIT_INTERVAL = 100 * 1000; // 100 ms

NameServerFetchThread::NameServerFetchThread() : recover_ret_(common::OB_SUCCESS), is_recover_done_(false), log_manager_(NULL) {
}

int NameServerFetchThread::wait_recover_done() {
  int count = 0;
  while (!is_recover_done_ && !_stop) {
    count++;
    if ((count % 100) == 0) {
      TBSYS_LOG(INFO, "wait recover from check point used %d seconds", (count / 10));
    }
    usleep(WAIT_INTERVAL);
  }
  TBSYS_LOG(INFO, "wait recover done:recover_ret[%d], recover_done[%d], stop[%d]", recover_ret_, is_recover_done_, _stop);
  return recover_ret_;
}

int NameServerFetchThread::got_ckpt(uint64_t ckpt_id) {

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

void NameServerFetchThread::set_log_manager(NameServerLogManager* log_manager) {
  log_manager_ = log_manager;
}

} /* nameserver */
} /* oceanbase */