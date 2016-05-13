/**
 * Copyright (c) 2012 Taobao.com
 * All rights reserved.
 *
 * �ļ����ƣ�MsgWrapper.h
 * ժҪ����ʽ��ϢWrapper
 * ���ߣ�Benkong <benkong@taobao.com>
 * ���ڣ�2013.5.16
 */
#ifndef _MSG_WRAPPER_H_
#define _MSG_WRAPPER_H_

#include <sys/types.h>

// �ַ��������װ
class IStrArray {
 public:
  virtual ~IStrArray() {}

  virtual size_t size() = 0;
  virtual int elementAt(int i, const char*& s, size_t& length) = 0;
  virtual const char* operator[](int i) = 0;
 protected:
  IStrArray() {}
};

// ������װ

#endif

