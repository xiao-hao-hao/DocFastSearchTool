#include "Common.h"
#include "Sysutil.h"
#include "./sqlite/sqlite3.h"

//引入静态库
#pragma comment(lib, "./sqlite/sqlite3.lib")

void Test_DirectionList()
{
	const string &path = "D:\\bishe";
	vector<string> subfile, subdir;
	DirectionList(path, subfile, subdir);
	for (const auto &e : subfile)
		cout << e << endl;
	for (const auto &e : subdir)
		cout << e  << endl;
}

//int sqlite3_get_table(
//	sqlite3 *db,          /* An open database */
//	const char *zSql,     /* SQL to be evaluated */
//	char ***pazResult,    /* Results of the query */
//	int *pnRow,           /* Number of result rows written here */
//	int *pnColumn,        /* Number of result columns written here */
//	char **pzErrmsg       /* Error msg written here */
//	);
//void sqlite3_free_table(char **result);

void Test_Sqlite()
{
	//数据库打开
	sqlite3 *db;
	int rc = sqlite3_open("test.db", &db);
	if (rc != SQLITE_OK)
		printf("Open database failed.\n");
	else
		printf("Open database successed\n");

	//操作数据库
	string sql = "select * from you_tb";
	char **result;
	int row, col;
	char *zErrmsg = 0;
	sqlite3_get_table(db, sql.c_str(), &result, &row, &col, &zErrmsg);//得到的数据是一个一维数组

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			printf("%-5s", *(result + (i*col) + j));
		}
		cout << endl;
	}

	sqlite3_free_table(result);
	//关闭数据库
	sqlite3_close(db);

}

int main()
{
	Test_Sqlite();
	//Test_DirectionList();
	return 0;
}

/*
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
int i;
for (i = 0; i<argc; i++)
{
printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
}
printf("\n");
return 0;
}

void Test_Sqlite()//测试数据库
{
	//数据库打开
	sqlite3 *db;
	int rc = sqlite3_open("test.db", &db);
	if (rc != SQLITE_OK)
		printf("Open database failed.\n");
	else
		printf("Open database successed\n");

	//操作数据库
	char *zErrMsg = 0;
	const char* str = "\nCallback function called\n";

	//sqlite3_exec(sqlite3*, const char *sql, sqlite_callback, void *data, char **errmsg)
	//string sql = "create table you_tb(id integer, name varchar(20), path varchar(100))";
	//string sql = "insert into you_tb values(1, 'abc', 'c:\\')";
	string sql = "select * from you_tb";
	//rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);
	rc = sqlite3_exec(db, sql.c_str(), callback, (void*)str, &zErrMsg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		//fprintf(stdout, "Table created successfully\n");
		fprintf(stdout, "Select data successfully\n");
	}
	sqlite3_close(db);
}


int main()
{
	Test_Sqlite();
	//Test_DirectionList();
	return 0;
}

*/