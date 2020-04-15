#pragma once

#include "Common.h"

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