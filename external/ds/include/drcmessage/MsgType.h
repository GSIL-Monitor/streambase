/**
 * Copyright (c) 2013 Taobao.com
 * All rights reserved.
 *
 * �ļ����ƣ�MsgType.h
 * ժҪ���������Ͷ���
 * ���ߣ�Benkong <benkong@taobao.com>
 * ���ڣ�2013.5.14
 */
#ifndef _MSG_TYPE_H_
#define _MSG_TYPE_H_

#include <stdint.h>

// ��������
enum {
  // ��������
  DT_UNKNOWN = 0x0000,
  DT_INT8   = 0x0001,
  DT_UINT8  = 0x0002,
  DT_INT16  = 0x0003,
  DT_UINT16 = 0x0004,
  DT_INT32  = 0x0005,
  DT_UINT32 = 0x0006,
  DT_INT64  = 0x0007,
  DT_UINT64 = 0x0008,
  DT_FLOAT  = 0x0009,
  DT_DOUBLE = 0x000a,
  DT_STRING = 0x000b,
  TOTAL_DT,
  DT_MASK   = 0x000F,

  // �������
  DC_ARRAY  = 0x0010,
  DC_NULL   = 0x0020,
  DC_MASK   = 0x0030,

  /*
  // ��������
  DA_FIXED_AREA = 0x0040,
  DA_VAR_AREA   = 0x0080,
  DA_EXT_AREA   = 0x00c0,
  DA_MASK       = 0x00c0
  */
};

const char STUFF_CHAR = '\0';
typedef uint8_t DT_TYPE;     //������������
typedef uint32_t STRLEN_TYPE;//�����ַ�������
typedef uint32_t COUNT_TYPE; //�������
typedef uint32_t OFFSET_TYPE;//����ƫ��

class MsgType {
 public:
  static int getValType(const char* typeName);
};

#endif

