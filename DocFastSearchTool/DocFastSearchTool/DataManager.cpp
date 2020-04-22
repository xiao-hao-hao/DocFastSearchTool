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

////////////////////////////////////////////////////////////////////////
//将结果表的释放封装到析构函数中
AutoGetResultTable::AutoGetResultTable(SqliteManager *db, const string &sql, int &row, 
									   int &col, char **&ppRet) :m_db(db)
{
	m_db->GetResultTable(sql, row, col, ppRet);
	m_ppRet = ppRet;
}
AutoGetResultTable::~AutoGetResultTable()
{
	if (m_ppRet)
		sqlite3_free_table(m_ppRet);
}
////////////////////////////////////////////////////////////////////////

DataManager& DataManager::GetInstance()
{
	static DataManager inst;//数据库在这里创建
	return inst;
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

	sprintf(sql, "create table if not exists %s(id integer primary key autoincrement, doc_name text, doc_path text, doc_pinyin text, doc_initials text)", DOC_TABLE);
	m_dbmgr.ExecuteSql(sql);
}
void DataManager::InsertDoc(const string &path, const string &doc)
{
	char sql[SQL_BUFFER_SIZE] = {0};
	string pinyin = ChineseConvertPinYinAllSpell(doc);
	string initials = ChineseConvertPinYinInitials(doc);
	sprintf(sql, "insert into %s values(null, '%s', '%s', '%s', '%s')", 
		DOC_TABLE, doc.c_str(), path.c_str(), pinyin.c_str(), initials.c_str());
	m_dbmgr.ExecuteSql(sql);
}
void DataManager::GetDocs(const string &path, multiset<string> &docs)
{
	char sql[SQL_BUFFER_SIZE] = {0};
	sprintf(sql, "select doc_name from %s where doc_path='%s'", DOC_TABLE, path.c_str());

	int row = 0, col = 0;
	char **ppRet = 0;
	//m_dbmgr.GetResultTable(sql, row, col, ppRet);
	AutoGetResultTable at(&m_dbmgr, sql, row, col, ppRet);

	for (int i = 1; i <= row; ++i)
		docs.insert(*(ppRet+i));

	//释放结果表
	//sqlite3_free_table(ppRet);
}
void DataManager::DeleteDoc(const string &path, const string &doc)
{
	char sql[SQL_BUFFER_SIZE] = {0};
	sprintf(sql, "delete from %s where doc_name='%s' and doc_path='%s'", DOC_TABLE, doc.c_str(), path.c_str());
	m_dbmgr.ExecuteSql(sql);

	///////////////////////////////
	//级联删除目录下的子文件
	string doc_path = path; //D:bishe
	doc_path += "\\";       //D:bishe\
	doc_path += doc;        //D:bishe\test

	memset(sql, 0, SQL_BUFFER_SIZE);
	sprintf(sql, "delete from %s where doc_path like '%s%%'", DOC_TABLE, doc_path.c_str());
	m_dbmgr.ExecuteSql(sql);
}

/////////////////////////////////////////////////////////////////////////
void  DataManager::Search(const string &key, vector<pair<string, string> > &doc_path)
{
	char sql[SQL_BUFFER_SIZE] = {0};
	//sprintf(sql, "select doc_name, doc_path from %s where doc_name like '%%%s%%'", DOC_TABLE, key.c_str());
	string pinyin = ChineseConvertPinYinAllSpell(key);
	string initials = ChineseConvertPinYinInitials(key);
	sprintf(sql, "select doc_name, doc_path from %s where doc_pinyin like '%%%s%%' or doc_initials like '%%%s%%'", 
		DOC_TABLE, pinyin.c_str(), initials.c_str());
	int row = 0, col = 0;
	char **ppRet = nullptr;

	//m_dbmgr.GetResultTable(sql, row, col,ppRet);
	AutoGetResultTable at(&m_dbmgr, sql, row, col, ppRet);

	doc_path.clear();
	//doc_name       doc_path
	//  1.txt         c:\ 
	//第一列和第二列制作一个值对结构然后插入
	for (int i = 1; i <= row; ++i)
		doc_path.push_back(make_pair(ppRet[i*col], ppRet[i*col+1]));

	//释放结果表
	//sqlite3_free_table(ppRet);
}

void DataManager::SplitHighlight(const string &str, const string &key,
	                            string &prefix, string &highlight, string &suffix)
{
	string strlower(str), keylower(key);
	//从什么地方开始转换，转换到什么地方，以什么方式转换，将字符串转换为小写，方便查找
	transform(strlower.begin(), strlower.end(), strlower.begin(), ::tolower);
	transform(keylower.begin(), keylower.end(), keylower.begin(), ::tolower);
	cout << strlower;
	//1 如果中文搜索，并能搜索成功，则直接分离
	size_t pos = strlower.find(keylower);//找到关键字的首位置
	if (pos != string::npos)//对原字符串进行分割
	{
		prefix = str.substr(0, pos);
		highlight = str.substr(pos, keylower.size());
		suffix = str.substr(pos+keylower.size(), string::npos);
		return;
	}

	//2 使用拼音全拼搜索

	//3 使用首字母搜索
}