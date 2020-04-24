#include "Common.h"
#include "Sysutil.h"
#include "./sqlite/sqlite3.h"
#include "DataManager.h"
#include "ScanManager.h"

char *title = "文档快速搜索工具";


int main(int argc, char *argv[])
{
	//HideCursor();
	const string &path = "D:\\bishe";
	//创建扫描实例
	ScanManager::CreateInstance(path);
	//sm.ScanDirectory(path);

	//创建搜索实例
	DataManager &dm = DataManager::GetInstance();
	vector<pair<string, string> > doc_path;
	string key;
	while (1)
	{
		DrawFrame(title);
		DrawMenu();
		cin >> key;
		if (key == "exit")
			break;
		dm.Search(key, doc_path);

		int init_row = 5;//由界面决定
		int count = 0;
		string prefix, highlight, suffix;
		for (const auto &e : doc_path)
		{
			string doc_name = e.first;
			string doc_path = e.second;
			DataManager::SplitHighlight(doc_name, key, prefix, highlight, suffix);
			SetCurPos(2, init_row + (count));
			cout << prefix;
			ColorPrintf(highlight.c_str());
			cout << suffix;
			SetCurPos(33, init_row + (count++));
			cout << doc_path;
			//printf("%-31s%-50s\n", e.first.c_str(), e.second.c_str());
		}
		HideCursor();
		SystemEnd();
		system("pause");
	}
	HideCursor();
	SystemEnd();
	return 0;
}



//void Test_HighLignt()
//{
//	//string str = "123abc比特科技，文档快速搜索工具，xyM服务赵";
//	string str = "比特科技";
//	string key = "b";
//	//string key = "ye";
//	string prefix, highlight, suffix;
//	DataManager::SplitHighlight(str, key, prefix, highlight, suffix);
//	cout << prefix;
//	ColorPrintf(highlight.c_str());
//	cout << suffix <<endl;
//}
//
//int main()
//{
//	system("color F0");
//	Test_HighLignt();
//	return 0;
//}


//void Test_DirectionList()
//{
//	const string &path = "D:\\bishe";
//	vector<string> subfile, subdir;
//	DirectionList(path, subfile, subdir);
//	for (const auto &e : subfile)
//		cout << e << endl;
//	for (const auto &e : subdir)
//		cout << e  << endl;
//}

//int sqlite3_get_table(
//	sqlite3 *db,          /* An open database */
//	const char *zSql,     /* SQL to be evaluated */
//	char ***pazResult,    /* Results of the query */
//	int *pnRow,           /* Number of result rows written here */
//	int *pnColumn,        /* Number of result columns written here */
//	char **pzErrmsg       /* Error msg written here */
//	);
//void sqlite3_free_table(char **result);

//void Test_Sqlite()
//{
//	//数据库打开
//	sqlite3 *db;
//	int rc = sqlite3_open("test.db", &db);
//	if (rc != SQLITE_OK)
//		printf("Open database failed.\n");
//	else
//		printf("Open database successed\n");
//
//	//操作数据库
//	string sql = "select * from you_tb";
//	char **result;
//	int row, col;
//	char *zErrmsg = 0;
//	sqlite3_get_table(db, sql.c_str(), &result, &row, &col, &zErrmsg);//得到的数据是一个一维数组
//
//	for (int i = 0; i <= row; ++i)
//	{
//		for (int j = 0; j < col; ++j)
//		{
//			printf("%-5s", *(result + (i*col) + j));
//		}
//		cout << endl;
//	}
//
//	sqlite3_free_table(result);
//	//关闭数据库
//	sqlite3_close(db);
//
//}

//void Test_SqliteManager()
//{
//	SqliteManager sm;
//	sm.Open("doc.db");
//	string sql = "create table if not exists doc_tb(id integer primary key autoincrement, doc_name text, doc_path text)";
//	sm.ExecuteSql(sql);
//	string sql1 = "insert into doc_tb values(null, 'stl.pdf', 'c:\\')";
//	sm.ExecuteSql(sql1);
//	string sql2 = "select * from doc_tb";
//	int row = 0, col = 0;
//	char **ppRet = nullptr;
//	sm.GetResultTable(sql2, row, col, ppRet);
//	for (int i = 0; i <= row; ++i)
//	{
//		for (int j = 0; j < col; ++j)
//		{
//			printf("%-10s", *(ppRet+(i*col)+j));
//		}
//		printf("\n");
//	}
//	sqlite3_free_table(ppRet);
//}
//
//void Test_Log()
//{
//	FILE *fp = fopen("Test.txt", "w");
//	if (fp == NULL)
//	{
//		TRACE_LOG("Open File Error.");
//		return;
//	}
//	else
//		TRACE_LOG("Open File Success.");
//
//	fclose(fp);
//}
//
//void Test_Search()
//{
//	const string &path = "D:\\bishe";
//	
//	//创建扫描实例
//	ScanManager::CreateInstance(path);
//	//sm.ScanDirectory(path);
//
//	//创建搜索实例
//	DataManager &dm = DataManager::GetInstance();
//
//	string key;
//	vector<pair<string, string> > doc_path;
//	while (1)
//	{
//		cout << "请输入要搜索的关键字:>";
//		cin >> key;
//		dm.Search(key, doc_path);
//
//		//显示结果
//		printf("%-15s%-50s\n", "名称", "路径");
//		for (const auto &e : doc_path)
//			printf("%-15s%-50s\n", e.first.c_str(), e.second.c_str());
//	}
//}
//
//void Test_Thread()
//{
//	thread th;
//}
//
//void Test_ChineseConvertPinYin()
//{
//	string str = "张三";
//	string pinyin = ChineseConvertPinYinAllSpell(str);
//	cout << "pinyin = " << pinyin << endl;
//
//	string initials = ChineseConvertPinYinInitials(str);
//	cout << "initials = " << initials << endl;
//}
//
//void Test_Frame()
//{
//	//system("mode con cols=60 lines=10");
//	//SetCurPos(4, (60-strlen("Hello Bit."))/2);
//	//printf("Hello Bit.\n");
//	char *title = "文档快速搜索工具";
//	DrawFrame(title);
//}
//
//int main()
//{
//	Test_Frame();
//	//Test_ChineseConvertPinYin();
//	//Test_Log();
//	//Test_Sqlite();
//	//Test_DirectionList();
//	//Test_SqliteManager();
//	//Test_Scan();
//	//Test_Search();
//	return 0;
//}

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