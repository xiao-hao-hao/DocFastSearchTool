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

#define DOC_DB    "doc.db"
#define DOC_TABLE "doc_tb"

class DataManager//对数据库文件系统的增删查改
{
public:
	DataManager();
	~DataManager();
public:
	void InitSqlite();
public:
	void InsertDoc(const string &path, const string &doc);
	void GetDocs(const string &path, multiset<string> &docs);
	void DeleteDoc(const string &path, const string &doc);

private:
	SqliteManager m_dbmgr;
};