#pragma once

#include "Common.h"
#include "Sysutil.h"

class SqliteManager//数据库的封装类
{
public:
	SqliteManager();
	~SqliteManager();
public:
	void Open(const string &path);//打开数据库
	void Close();//关闭数据库
	void ExecuteSql(const string &sql);//执行数据库语句
	void GetResultTable(const string &sql, int &row, int &col, char **&ppRet);//获得查询结果
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
	//c++11的方式，不允许拷贝构造，不允许赋值
	AutoGetResultTable(const AutoGetResultTable &) = delete;
	AutoGetResultTable& operator=(const AutoGetResultTable &) = delete;
//不允许拷贝构造，不允许赋值
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
//单例化：构造函数私有化，创建一个获得实例的方法
class DataManager//对数据库文件系统的增删查改
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