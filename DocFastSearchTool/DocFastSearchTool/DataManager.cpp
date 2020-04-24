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
	//cout << strlower;
	//1 如果中文搜索，并能搜索成功，则直接分离
	size_t pos = strlower.find(keylower);//找到关键字的首位置
	if (pos != string::npos)//对原字符串进行分割
	{
		prefix = str.substr(0, pos);
		highlight = str.substr(pos, keylower.size());
		suffix = str.substr(pos+keylower.size(), string::npos);//第二个参数表示返回的字节个数，如果是负数，表示一直返回直到字符串结束位置
		return;
	}

	//2 使用拼音全拼搜索，则需要匹配分离子串汉字和拼音
	//注意一个汉字占两个字节，但是一个汉字的拼音占几个字节不确定，需要求一下
	string str_pinyin = ChineseConvertPinYinAllSpell(strlower);
	string key_pinyin = ChineseConvertPinYinAllSpell(keylower);//key可能是一半拼音一半汉字
	pos = str_pinyin.find(key_pinyin);//在str中找key的位置
	if (pos != string::npos)//如果找到了
	{
		size_t str_index = 0;
		size_t pinyin_index = 0;

		size_t highlight_index = 0;
		size_t highlight_len = 0;

		while (str_index < strlower.size())
		{
			if (pinyin_index == pos)
			{
				highlight_index = str_index;//找到高亮位置索引

			}
			if (pinyin_index == pos + key_pinyin.size())//当pinyin_index大于高亮部分索引时把长度计算出来
			{
				highlight_len = str_index - highlight_index;//求得高亮部分长度
				break;
			}
			if (strlower[str_index]>=0 && strlower[str_index]<=127)
			{
				//是一个字符，索引移动一个字节
				++str_index;
				++pinyin_index;
			}
			else
			{
				//是汉字
				string word(strlower, str_index, 2);//提取一个汉字
				string word_pinyin = ChineseConvertPinYinAllSpell(word);//提取的汉字对应的拼音

				str_index += 2;//文字跨越的字节数
				pinyin_index += word_pinyin.size();//拼音跨越的字节数
			}
		}
		//注意是对原字符串分割，不是对拼音字符串分割
		if (str_index == strlower.size())//考虑搜索的是最后一个字的情况
			highlight_len = str_index - highlight_index;
		prefix = str.substr(0, highlight_index);
		highlight = str.substr(highlight_index, highlight_len);
		suffix = str.substr(highlight_index+highlight_len, string::npos);
		return;
	}

	//3 使用首字母搜索
	string str_initials = ChineseConvertPinYinInitials(strlower);
	string key_initials = ChineseConvertPinYinInitials(keylower);
	pos = str_initials.find(key_initials);
	if (pos != string::npos)
	{
		size_t str_index = 0;
		size_t initials_index = 0;

		size_t highlight_index = 0;
		size_t highlight_len = 0;

		while (str_index < strlower.size())
		{
			if (initials_index == pos)
			{
				highlight_index = str_index;
			}
			if (initials_index == pos + key_initials.size())
			{
				highlight_len = str_index - highlight_index;
				break;
			}
			if (strlower[str_index]>=0 && strlower[str_index]<=127)
			{
				//是一个字符，索引移动一个字节
				++str_index;
				++initials_index;
			}
			else
			{
				//是一个汉字
				if (str_initials[initials_index]<0 || str_initials[initials_index]>127)
					initials_index += 2;
				else
					++initials_index;
				str_index += 2;
			}
		}
		if (str_index == strlower.size())
			highlight_len = str_index - highlight_index;
		prefix = str.substr(0, highlight_index);
		highlight = str.substr(highlight_index, highlight_len);
		suffix = str.substr(highlight_index + highlight_len, string::npos);
		return;
	}


	//没有找到的情况
	prefix = str;
	highlight.clear();
	suffix.clear();
}