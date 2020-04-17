#include "ScanManager.h"
#include "Sysutil.h"

void ScanManager::ScanDirectory(const string &path)
{
	//1 扫描本地文件系统
	vector<string> local_files; //subfile
	vector<string> local_dirs; //subdirs
	DirectionList(path, local_files, local_dirs);
	set<string> local_set;
	local_set.insert(local_files.begin(), local_files.end());
	local_set.insert(local_dirs.begin(), local_dirs.end());

	//2 扫描数据库文件系统
	multiset<string> db_set;
	m_db.GetDocs(path, db_set);

	//3 对比本地文件与数据库文件(会牵扯到增删查改的过程)
	auto local_it = local_set.begin();
	auto db_it = db_set.begin();
	while (local_it!=local_set.end() && db_it!=db_set.end())
	{
		if (*local_it < *db_it)
		{
			//本地文件存在，数据库文件不存在，数据库需要新增文件
			m_db.InsertDoc(path, *local_it);
			++local_it;
		}
		else if (*local_it > *db_it)
		{
			//本地文件不存在，数据库文件存在，数据库需要删除文件
			m_db.DeleteDoc(path, *db_it);
			++db_it;
		}
		else
		{
			//本地文件存在，数据库文件存在，数据库文件不需要改变
			++local_it;
			++db_it;
		}
	}

	while (local_it != local_set.end())
	{
		//本地文件存在，数据库文件不存在，数据库需要新增文件
		m_db.InsertDoc(path, *local_it);
		++local_it;
	}
	while (db_it != db_set.end())
	{
		//本地文件不存在，数据库文件存在，数据库需要删除文件
		m_db.DeleteDoc(path, *db_it);
		++db_it;
	}

	//递归遍历子目录
}