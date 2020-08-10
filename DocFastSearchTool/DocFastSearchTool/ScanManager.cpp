#include "ScanManager.h"
#include "Sysutil.h"

ScanManager::ScanManager()
{}
void ScanManager::StartScan(const string &path)
{
	while (1)
	{		
		//等待3秒再扫描,阻塞当前线程rel_time的时间
		//this_thread::sleep_for(chrono::seconds(3));
		ScanDirectory(path);
	}
}
ScanManager& ScanManager::CreateInstance(const string &path1, const string &path2, const string &path3)
{
	static ScanManager inst;
	thread scan_thread1(&StartScan, &inst, path1);
	thread scan_thread2(&StartScan, &inst, path2);
	thread scan_thread3(&StartScan, &inst, path3);
	scan_thread1.detach();//与主线程分离
	scan_thread2.detach();
	scan_thread3.detach();
	return inst;
}

void ScanManager::ScanDirectory(const string &path)
{
	//1 扫描本地文件系统
	//如果本地文件系统把一个目录删了，那么local_dirs中就不会有
	//这个目录，在递归遍历的时候不会遍历他，所以不会出错
	vector<string> local_files; //subfile
	vector<string> local_dirs; //subdirs
	DirectoryList(path, local_files, local_dirs);
	set<string> local_set;
	local_set.insert(local_files.begin(), local_files.end());
	local_set.insert(local_dirs.begin(), local_dirs.end());

	//2 扫描数据库文件系统
	DataManager &m_db = DataManager::GetInstance();

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
	for (const auto &dir : local_dirs)
	{
		string dir_path = path;
		dir_path += "\\";
		dir_path += dir;
		ScanDirectory(dir_path);
	}
}