#pragma once

#include "Common.h"

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