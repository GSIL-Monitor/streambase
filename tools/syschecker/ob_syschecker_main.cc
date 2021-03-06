/**
 * (C) 2010 Taobao Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * ob_syschecker_main.cc for define syschecker main scheduler.
 *
 * Authors:
 *   huating <huating.zmq@taobao.com>
 *
 */
#include <new>
#include "ob_syschecker_main.h"
#include "common/ob_malloc.h"

namespace sb {
namespace syschecker {
using namespace common;

ObSyscheckerMain::ObSyscheckerMain() {

}

ObSyscheckerMain* ObSyscheckerMain::get_instance() {
  if (NULL == instance_) {
    instance_ = new(std::nothrow) ObSyscheckerMain();
  }

  return dynamic_cast<ObSyscheckerMain*>(instance_);
}

int ObSyscheckerMain::do_work() {
  return checker_.start();
}

void ObSyscheckerMain::do_signal(const int sig) {
  switch (sig) {
  case SIGTERM:
  case SIGINT:
    checker_.stop();
    break;
  default:
    break;
  }
}
} // end namespace syschecker
} // end namespace sb
