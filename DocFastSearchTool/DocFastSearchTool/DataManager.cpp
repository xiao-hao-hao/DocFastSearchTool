#include "DataManager.h"

//引入静态库
#pragma comment(lib, "./sqlite/sqlite3.lib")

SqliteManager::SqliteManager() : m_db(nullptr)
{}

SqliteManager::~SqliteManager()
{
	Close();
}

void SqliteManager::Open(const string &path)//打开数据库
{
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(path.c_str(), &m_db);
	if (rc)
	{
		//fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_db));
		ERROR_LOG("Can't open database: %s", sqlite3_errmsg(m_db));
		exit(0);
	}
	else
	{
		//fprintf(stderr, "Opened database successfully\n");
		TRACE_LOG("Opened database successfully");
	}
}

void SqliteManager::Close()//关闭数据库
{
	if (m_db)
	{
		int rc = sqlite3_close(m_db);
		if (rc != SQLITE_OK)
		{
			//fprintf(stderr, "Close database faled: %s\n", sqlite3_errmsg(m_db));
			ERROR_LOG("Close database faled: %s", sqlite3_errmsg(m_db));
		}
		else
			//fprintf(stderr, "Close database successfully\n");
			TRACE_LOG("Close database successfully");
	}
}
void SqliteManager::ExecuteSql(const string &sql)//执行数据库语句
{
	char *zErrMsg = 0;
	int rc = sqlite3_exec(m_db, sql.c_str(), 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		//fprintf(stderr, "SQL(%s) error: %s\n", sql.c_str(), zErrMsg);
		ERROR_LOG("SQL(%s) error: %s", sql.c_str(), zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		//fprintf(stdout, "SQL(%s) Execute successfully\n", sql.c_str());
		TRACE_LOG("SQL(%s) Execute successfully", sql.c_str());
	}
}
void SqliteManager::GetResultTable(const string &sql, int &row, int &col, char **&ppRet)//获得查询结果
{
	char *zErrMsg = 0;
	int rc = sqlite3_get_table(m_db, sql.c_str(), &ppRet, &row, &col, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		//fprintf(stderr, "Get Result Table error %s\n", zErrMsg);
		ERROR_LOG("Get Result Table error %s", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		//fprintf(stderr, "Get Result Table successfully\n");
		TRACE_LOG("Get Result Table successfully");
	}
}



DataManager::DataManager()
{
	m_dbmgr.Open(DOC_DB);
	InitSqlite();
}
DataManager::~DataManager()
{}
void DataManager::InitSqlite()
{
	char sql[SQL_BUFFER_SIZE] = {0};

	sprintf(sql, "create table if not exists %s(id integer primary key autoincrement, doc_name text, doc_path text)", DOC_TABLE);
	m_dbmgr.ExecuteSql(sql);
}
void DataManager::InsertDoc(const string &path, const string &doc)
{
	char sql[SQL_BUFFER_SIZE] = {0};
	sprintf(sql, "insert into %s values(null, '%s', '%s')", DOC_TABLE, doc.c_str(), path.c_str());
	m_dbmgr.ExecuteSql(sql);
}
void DataManager::GetDocs(const string &path, multiset<string> &docs)
{
	char sql[SQL_BUFFER_SIZE] = {0};
	sprintf(sql, "select doc_name from %s where doc_path='%s'", DOC_TABLE, path.c_str());

	int row = 0, col = 0;
	char **ppRet = 0;
	m_dbmgr.GetResultTable(sql, row, col, ppRet);

	for (int i = 1; i <= row; ++i)
		docs.insert(*ppRet);////////////////////?????????????

	//释放结果表
	sqlite3_free_table(ppRet);
}
void DataManager::DeleteDoc(const string &path, const string &doc)
{
	char sql[SQL_BUFFER_SIZE] = {0};
	sprintf(sql, "delete from %s where doc_name='%s' and doc_path='%s'", DOC_TABLE, doc.c_str(), path.c_str());
	m_dbmgr.ExecuteSql(sql);
}