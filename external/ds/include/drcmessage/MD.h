/**
 * Copyright (c) 2012 Taobao.com
 * All rights reserved.
 *
 * �ļ����ƣ�MD.h
 * ժҪ��db��table��column��meta��Ϣ��ȡ�ӿ�
 * ���ߣ�Benkong <benkong@taobao.com>
 * ���ڣ�2013.5.22
 */
#ifndef _MD_H_
#define _MD_H_

#include "MsgWrapper.h"
#include <string>
#include <vector>
#include <time.h>

enum drcmsg_field_types {
  DRCMSG_TYPE_DECIMAL, DRCMSG_TYPE_TINY,
  DRCMSG_TYPE_SHORT,  DRCMSG_TYPE_LONG,
  DRCMSG_TYPE_FLOAT,  DRCMSG_TYPE_DOUBLE,
  DRCMSG_TYPE_NULL,   DRCMSG_TYPE_TIMESTAMP,
  DRCMSG_TYPE_LONGLONG, DRCMSG_TYPE_INT24,
  DRCMSG_TYPE_DATE,   DRCMSG_TYPE_TIME,
  DRCMSG_TYPE_DATETIME, DRCMSG_TYPE_YEAR,
  DRCMSG_TYPE_NEWDATE, DRCMSG_TYPE_VARCHAR,
  DRCMSG_TYPE_BIT,
  DRCMSG_TYPE_NEWDECIMAL = 246,
  DRCMSG_TYPE_ENUM = 247,
  DRCMSG_TYPE_SET = 248,
  DRCMSG_TYPE_TINY_BLOB = 249,
  DRCMSG_TYPE_MEDIUM_BLOB = 250,
  DRCMSG_TYPE_LONG_BLOB = 251,
  DRCMSG_TYPE_BLOB = 252,
  DRCMSG_TYPE_VAR_STRING = 253,
  DRCMSG_TYPE_STRING = 254,
  DRCMSG_TYPE_GEOMETRY = 255,
  DRCMSG_TYPES
};

// column��������Ϣ
class IColMeta {
 public:
  virtual const char* getName() = 0;
  virtual int getType() = 0;
  virtual long getLength() = 0;
  virtual bool isSigned() = 0;
  virtual bool isPK() = 0;
  virtual bool isUK() { return false; }
  virtual bool isNotNull() = 0;
  virtual int getDecimals() = 0;
  virtual const char* getDefault() = 0;
  virtual const char* getEncoding() = 0;
  virtual int getRequired() = 0;
  virtual IStrArray* getValuesOfEnumSet() = 0; // ʹ��IStrArray����Ҫdelete

  virtual void setName(const char* name) = 0;
  virtual void setType(int type) = 0;
  virtual void setLength(long length) = 0;
  virtual void setSigned(bool b) = 0;
  virtual void setIsPK(bool b) = 0;
  virtual void setIsUK(bool b) { (void)(b); return; }
  virtual void setNotNull(bool b) = 0;
  virtual void setDecimals(int decimals) = 0;
  virtual void setDefault(const char* def) = 0;
  virtual void setEncoding(const char* enc) = 0;
  virtual void setRequired(int required) = 0;
  virtual void setValuesOfEnumSet(std::vector<std::string>& v) = 0;
  virtual void setValuesOfEnumSet(std::vector<const char*>& v) = 0;
  virtual void setValuesOfEnumSet(const char** v, size_t size) = 0;

 public:
  IColMeta() {}
  virtual ~IColMeta() {}
};

// table��������Ϣ
class IDBMeta;
class ITableMeta {
 public:
  virtual const char* getName() = 0;
  virtual bool hasPK() = 0;
  virtual bool hasUK() { return false; }
  virtual const char* getPKs() = 0;
  virtual const char* getUKs() { return NULL; }
  virtual const char* getEncoding() = 0;
  virtual IDBMeta* getDBMeta() = 0;

  virtual void setName(const char* name) = 0;
  virtual void setHasPK(bool b) = 0;
  virtual void setHasUK(bool b) { (void)(b); return; }
  virtual void setPKs(const char* pks) = 0;
  virtual void setUKs(const char* uks) { (void)uks; return; }
  virtual void setEncoding(const char* enc) = 0;
  virtual void setDBMeta(IDBMeta* dbMeta) = 0;

 public:

  /**
   * ��������ֶ���
   */
  virtual std::vector<std::string>& getColNames() = 0;

  /**
   * ���primary key��
   */
  virtual std::vector<std::string>& getPKColNames() = 0;

  /**
  * ͨ��column�����ֻ�ȡĳ��column��meta��Ϣ
  * @param colName   column������
  * @return NULL: û��colName��Ӧ��������Ϣ; ��NULL: colName��Ӧ��������Ϣ
  */
  virtual IColMeta* getCol(const char* colName) = 0;

  /**
   * ��ȡColumn��Ŀ
   * @return Column��Ŀ
   */
  virtual int getColCount() = 0;

  /*
   * ͨ��column��mysql�е��ڲ���ŵõ�ĳ��column��meta��Ϣ
   *
   * @param index column�����к�
   * @return NULL: û��colName��Ӧ��������Ϣ; ��NULL: colName��Ӧ��������Ϣ
   */
  virtual IColMeta* getCol(int index) = 0;
  virtual int getColNum(const char* colName) = 0;

  /**
   * ׷��һ��column��������Ϣ
   * @param colName  column����
   * @param colMeta  column��������Ϣ
   * @return 0: �ɹ��� <0: ʧ��
   */
  virtual int append(const char* colName, IColMeta* colMeta) = 0;

 public:
  ITableMeta() {}
  virtual ~ITableMeta() {}
};

// DB��������Ϣ
class IMetaDataCollections;
class IDBMeta {
 public:
  virtual const char* getName() = 0;
  virtual const char* getEncoding() = 0;
  virtual IMetaDataCollections* getMetaDataCollections() = 0;

  virtual void setName(const char* name) = 0;
  virtual void setEncoding(const char* enc) = 0;
  virtual void setMetaDataCollections(IMetaDataCollections* mdc) = 0;

  /**
   * ��ȡTable��Ŀ
   * @return Table��Ŀ
   */
  virtual int getTblCount() = 0;

  /**
  * ��ȡָ��table��������Ϣ
  * @param tblName  table����
  * @return NULL: û��tblName��Ӧ��������Ϣ; ��NULL: tblName��Ӧ��������Ϣ
  */
  virtual ITableMeta* get(const char* tblName) = 0;

  /*
  * ͨ��Table��DB�е��ڲ���ŵõ�ĳ��table��meta��Ϣ
  *
  * @param index table�����к�
  * @return NULL: û��table��Ӧ��������Ϣ; ��NULL: index��Ӧ��������Ϣ
  */
  virtual ITableMeta* get(int index) = 0;

  /**
   * ����һ��table������Ϣ
   * @param tblName  table����
   * @param tblMeta  table������Ϣ
   * @return 0: �ɹ�; <0: ʧ��
   */
  virtual int put(const char* tblName, ITableMeta* tblMeta) = 0;

 public:
  IDBMeta() {}
  virtual ~IDBMeta() {}
};

// meta data���ϣ��������е�db��������Ϣ
class IMetaDataCollections {
 public:
  virtual unsigned getMetaVerNum() = 0;
  virtual IMetaDataCollections* getPrev() = 0;
  virtual time_t getTimestamp() = 0;

  virtual void setMetaVerNum(unsigned metaVerNum) = 0;
  virtual void setPrev(IMetaDataCollections* prev) = 0;
  virtual void setTimestamp(time_t timestamp) = 0;
 public:

  /**
   * ��ȡDb����Ŀ
   * @return Db����Ŀ
   */
  virtual int getDbCount() = 0;

  /**
   * ��ȡָ�����Ƶ�Db������Ϣ
   * @param dbname ָ������
   * @return ָ��Db��������Ϣ
   */
  virtual IDBMeta* get(const char* dbname) = 0;

  /**
   * ��ȡָ��λ�õ�Db������Ϣ
   * @param index ָ��λ��
   * @return ָ��Db��������Ϣ
   */
  virtual IDBMeta* get(int index) = 0;

  /**
   * ��ȡָ��table��������Ϣ
   * @param dbName   db����
   * @param tblName  table����
   * @return NULL: û��tblName��Ӧ��������Ϣ; ��NULL: tblName��Ӧ��������Ϣ
   */
  virtual ITableMeta* get(const char* dbName, const char* tblName) = 0;

  /**
   * ����һ��db������Ϣ
   * @param dbName  db����
   * @param dbMeta  db������Ϣ
   * @return 0: �ɹ�; <0: ʧ��
   */
  virtual int put(const char* dbName, IDBMeta* dbMeta) = 0;

 public:
  /**
   * ���л�Ϊ�ַ���
   * @param s  �������л���Ľ��
   * @return 0: �ɹ�; <0: ʧ��
   */
  virtual int toString(std::string& s) = 0;

  /**
   * ����ֻ����¼
     * @param ptr   ������
     * @param size  ptr��ռ�ֽ���
   * @return 0:�ɹ���<0: ʧ��
   */
  virtual int parse(const void* ptr, size_t size) = 0;

  /**
   * �����Ƿ�ɹ�
   * @return true/false
   */
  virtual bool parsedOK() = 0;

  /**
   * ֻ��������ʵ����Ч���ֽ�����parse()��ʱ������size�ȱ���Ч���ݳ�
   * @return �ֽ���
   */
  virtual size_t getRealSize() = 0;

 public:
  IMetaDataCollections() {}
  virtual ~IMetaDataCollections() {}
};

#endif

