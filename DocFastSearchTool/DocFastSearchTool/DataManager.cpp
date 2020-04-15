#include "DataManager.h"

//���뾲̬��
#pragma comment(lib, "./sqlite/sqlite3.lib")

SqliteManager::SqliteManager() : m_db(nullptr)
{}

SqliteManager::~SqliteManager()
{
	Close();
}

void SqliteManager::Open(const string &path)//�����ݿ�
{
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(path.c_str(), &m_db);
	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_db));
		exit(0);
	}
	else
	{
		fprintf(stderr, "Opened database successfully\n");
	}
}

void SqliteManager::Close()//�ر����ݿ�
{
	if (m_db)
	{
		int rc = sqlite3_close(m_db);
		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "Close database faled: %s\n", sqlite3_errmsg(m_db));
		}
		else
			fprintf(stderr, "Close database successfully\n");
	}
}
void SqliteManager::ExecuteSql(const string &sql)//ִ�����ݿ����
{
	char *zErrMsg = 0;
	int rc = sqlite3_exec(m_db, sql.c_str(), 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL(%s) error: %s\n", sql.c_str(), zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "SQL(%s) Execute successfully\n", sql.c_str());
	}
}
void SqliteManager::GetResultTable(const string &sql, int &row, int &col, char **&ppRet)//��ò�ѯ���
{
	char *zErrMsg = 0;
	int rc = sqlite3_get_table(m_db, sql.c_str(), &ppRet, &row, &col, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Get Result Table error %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stderr, "Get Result Table successfully\n");
	}
}