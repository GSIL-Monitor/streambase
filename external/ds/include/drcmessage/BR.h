/**
 * Copyright (c) 2012 Taobao.com
 * All rights reserved.
 *
 * �ļ����ƣ�BinlogRecord.h
 * ժҪ��ͨ��Binlog���������ļ�¼��Ϣ����
 * ���ߣ�Benkong <benkong@taobao.com>
 *       Jierui.lj <jierui.lj@taobao.com>
 * ���ڣ�2012.3.13
 *       2012.8.9 ͳһ���������ʽ���ֶκ�BinlogRecord, Binlog�Ĺ���
 *       2013.5.22 beknong ȥ���������������ݿ�ĸ�ʽ������
 */
#ifndef _BR_H_
#define _BR_H_

#include <string>
#include <vector>
#include <stdint.h>
class IStrArray;

#define BR_FAKE_DDL_COLNAME "ddl"

typedef enum RecordType {
  EINSERT = 0,
  EUPDATE,
  EDELETE,
  EREPLACE,
  HEARTBEAT,
  CONSISTENCY_TEST,
  EBEGIN,
  ECOMMIT,
  EDDL,
  EROLLBACK,
  EUNKNOWN,
} RECORD_TYPE;

enum FORMATERR {
  FORMATOK = 0,
  BUFOVERFLOW,
  TYPENOTSUPPORT,
  UNKNOWNTYPE,
  UNKNOWNOPTYPE,
  NULLTBLMETA,
  NULLCOLMETA,
  ENUMSETNULL
};

enum SOURCETYPE {
  SRC_MYSQL = 0,
  SRC_OCEANBASE,
  SRC_UNKNOWN
};

class ITableMeta;
class IBinlogRecord {
 public:

  virtual ~IBinlogRecord() {}

  /**
   * ��ȡ��¼���͵���Դ
   * @return ��Դ����
   */
  virtual int getSrcType() const = 0;

  /**
   * ����ֻ����¼
     * @param ptr   ������
     * @param size  ptr��ռ�ֽ���
   * @return 0:�ɹ���<0: ʧ��
   */
  virtual int parse(const void* ptr, size_t size) = 0;

  /**
   * �Ƿ�����ɹ�
   * @return true/false
   */
  virtual bool parsedOK() = 0;

  /**
   * ֻ��������ʵ����Ч���ֽ�����parse()��ʱ������size�ȱ���Ч���ݳ�
   * @return �ֽ���
   */
  virtual size_t getRealSize() = 0;

  /**
   * ������и���ǰ�ֶ�
   * @return 0: �ɹ� ��0 ʧ��
   */
  virtual void clearOld() = 0;

  /**
   * ������º���ֶ�
   * @return 0: �ɹ� ��0:ʧ��
   */
  virtual void clearNew() = 0;

  /**
   * ��ȡ����ǰcolumn��ֵ��ӳ���
   * @return column name => column value
   */
  virtual const std::vector<std::string*>& oldCols() = 0;
  virtual IStrArray* parsedOldCols() const = 0;

  /**
   * ��ȡ���º�column��ֵ��ӳ���
   * @return column name => column value
   */
  virtual const std::vector<std::string*>& newCols() = 0;
  virtual IStrArray* parsedNewCols() const = 0;

  /**
   * ��ȡ�ֶ���
   * @return ȫ���ֶ���
   */
  virtual IStrArray* parsedColNames() const = 0;

  /**
   * ��ȡȫ�ֶ�����
   * @return �ֶ�����
   */
  virtual const uint8_t* parsedColTypes() const = 0;

  /**
   * ��ȡip-port
   * @return ��Դ���ip-port
   */
  virtual const char* instance() const = 0;

  /**
   * ��ȡ��¼��Դ��db��
   * @return db��
   */
  virtual const char* dbname() const = 0;

  /**
   * ��ȡ��¼��Դ��table��
   * @return table��
   */
  virtual const char* tbname() const = 0;

  /**
   * ��ȡ�Զ��ŷָ��������ַ���
   * @return ȫ��������
   */
  virtual const char* pkValue() const = 0;
  virtual const char* ukValue() const { return NULL; }

  /**
   * �ж��Ƿ���������¼��ͻ
   */
  virtual bool conflicts(const IBinlogRecord*) = 0;

  /**
   * ���л�Ϊ�ַ���
   * @return ��NULL: �ɹ�; NULL: ʧ��
   */
  virtual std::string* toParsedString(int version = 2) = 0;
  virtual const std::string* toString() = 0;

  /**
   * ������������Ա�����
   */
  virtual void clear() = 0;

  /**
   * ���ؼ�¼����
   * @return ��¼��������
   */
  virtual int recordType() = 0;

  /**
   * ���ؼ�¼�ֶ����ݱ���
   * @return ����
   */
  virtual const char* recordEncoding() = 0;

  /**
   * ���ظ���־��¼����ʱ��
   * @return ����ʱ��
   */
  virtual time_t getTimestamp() = 0;

  /**
   * �õ���¼��λ����ַ�����ʽ
   * @return λ����ַ�����ʽ
   */
  virtual const char* getCheckpoint() = 0;

  /**
   * ��ȡ2��32λ��λ������, ����mysql��ob�Ĳ�ͬ
   * mysql: getCheckpoint1��õ����ļ����, getCheckpoint2 ��¼�ļ���ƫ��
   * ob: getCheckpoint1��32λcheckpoint, getCheckpoint2��32λ
   */
  virtual unsigned getCheckpoint1() = 0;
  virtual unsigned getCheckpoint2() = 0;

  /**
   * ��ȡ��¼��id
   * @return ��¼id
   */
  virtual uint64_t id() = 0;

  /**
  * ��ȡ��¼��Ӧ���meta��Ϣ
  * @return TableMeta��Ϣ
  */
  virtual ITableMeta* getTableMeta() = 0;

  /**
   * ��ȡ��¼��ʱ���ߣ������ܷ���
   * @param length ���鳤��
   * @return timemark ʱ������
   */
  virtual long* getTimemark(size_t& length) = 0;

  /**
   * ����һ��ʱ�����¼���������ܷ���
   * @param time ʱ���
   */
  virtual void addTimemark(long time) = 0;
};

#endif

