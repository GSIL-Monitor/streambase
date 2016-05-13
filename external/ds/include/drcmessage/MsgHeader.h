/**
 * Copyright (c) 2013 Taobao.com
 * All rights reserved.
 *
 * �ļ����ƣ�MsgHeader.h
 * ժҪ���������Ͷ���
 * ���ߣ�Benkong <benkong@taobao.com>
 * ���ڣ�2013.5.14
 */
#ifndef _MSG_HEADER_H_
#define _MSG_HEADER_H_

#include <stdint.h>

// ��Ϣ����
enum {
  MT_UNKNOWN = 0,
  MT_META,
  MT_FIXED,
  MT_VAR,
  MT_EXT
};

struct MsgHeader {
  uint16_t m_msgType;
  uint16_t m_version;
  uint32_t m_size;
};

#endif

