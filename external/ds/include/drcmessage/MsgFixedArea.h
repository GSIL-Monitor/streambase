/**
 * Copyright (c) 2012 Taobao.com
 * All rights reserved.
 *
 * �ļ����ƣ�MsgFixedArea.h
 * ժҪ����ʽ��Ϣ��ʽ�̶�����������
 * ���ߣ�Benkong <benkong@taobao.com>
 * ���ڣ�2013.5.14
 */
#ifndef _MSG_FIXED_AREA_H_
#define _MSG_FIXED_AREA_H_

#include <sys/types.h>

struct MFAInfo;
class MsgFixedArea {
 public:
  MsgFixedArea();
  MsgFixedArea(const void* ptr, size_t size);
  MsgFixedArea(size_t size);
  ~MsgFixedArea();
  int setField(size_t offset, const void* ptr, size_t size);
  void toBinary(const void*& ptr, size_t& size);

  void clear();
  int parse(const void* ptr, size_t size);
  size_t getRealSize();
  int getFieldPtr(size_t offset, const void*& ptr);
 private:
  MFAInfo* m_mfa;
};

#endif

