#pragma once

#include "Common.h"
#include "Sysutil.h"

class SqliteManager//���ݿ�ķ�װ��
{
public:
	SqliteManager();
	~SqliteManager();
public:
	void Open(const string &path);//�����ݿ�
	void Close();//�ر����ݿ�
	void ExecuteSql(const string &sql);//ִ�����ݿ����
	void GetResultTable(const string &sql, int &row, int &col, char **&ppRet);//��ò�ѯ���
private:
	sqlite3 *m_db;
};

///////////////////////////////////////////////////////////////////////////////
class AutoGetResultTable
{
public:
	AutoGetResultTable(SqliteManager *db, const string &sql, int &row, int &col, char **&ppRet);
	~AutoGetResultTable();
public:
	//c++11�ķ�ʽ�������������죬������ֵ
	AutoGetResultTable(const AutoGetResultTable &) = delete;
	AutoGetResultTable& operator=(const AutoGetResultTable &) = delete;
//�����������죬������ֵ
//protected:
//	AutoGetResultTable(const AutoGetResultTable &);
//	AutoGetResultTable& operator=(const AutoGetResultTable &);
private:
	SqliteManager *m_db;
	char          **m_ppRet;
};


///////////////////////////////////////////////////////////////////////////////


#define DOC_DB    "doc.db"
#define DOC_TABLE "doc_tb"
//�����������캯��˽�л�������һ�����ʵ���ķ���
class DataManager//�����ݿ��ļ�ϵͳ����ɾ���
{
public:
	static DataManager& GetInstance();
public:
	~DataManager();
public:
	void InitSqlite();
public:
	void InsertDoc(const string &path, const string &doc);
	void GetDocs(const string &path, multiset<string> &docs);
	void DeleteDoc(const string &path, const string &doc);
public:
	void  Search(const string &key, vector<pair<string, string> > &doc_path);
private:
	DataManager();
private:
	SqliteManager m_dbmgr;
};