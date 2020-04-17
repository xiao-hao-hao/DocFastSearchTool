#include "ScanManager.h"
#include "Sysutil.h"

void ScanManager::ScanDirectory(const string &path)
{
	//1 ɨ�豾���ļ�ϵͳ
	vector<string> local_files; //subfile
	vector<string> local_dirs; //subdirs
	DirectionList(path, local_files, local_dirs);
	set<string> local_set;
	local_set.insert(local_files.begin(), local_files.end());
	local_set.insert(local_dirs.begin(), local_dirs.end());

	//2 ɨ�����ݿ��ļ�ϵͳ
	multiset<string> db_set;
	m_db.GetDocs(path, db_set);

	//3 �Աȱ����ļ������ݿ��ļ�(��ǣ������ɾ��ĵĹ���)
	auto local_it = local_set.begin();
	auto db_it = db_set.begin();
	while (local_it!=local_set.end() && db_it!=db_set.end())
	{
		if (*local_it < *db_it)
		{
			//�����ļ����ڣ����ݿ��ļ������ڣ����ݿ���Ҫ�����ļ�
			m_db.InsertDoc(path, *local_it);
			++local_it;
		}
		else if (*local_it > *db_it)
		{
			//�����ļ������ڣ����ݿ��ļ����ڣ����ݿ���Ҫɾ���ļ�
			m_db.DeleteDoc(path, *db_it);
			++db_it;
		}
		else
		{
			//�����ļ����ڣ����ݿ��ļ����ڣ����ݿ��ļ�����Ҫ�ı�
			++local_it;
			++db_it;
		}
	}

	while (local_it != local_set.end())
	{
		//�����ļ����ڣ����ݿ��ļ������ڣ����ݿ���Ҫ�����ļ�
		m_db.InsertDoc(path, *local_it);
		++local_it;
	}
	while (db_it != db_set.end())
	{
		//�����ļ������ڣ����ݿ��ļ����ڣ����ݿ���Ҫɾ���ļ�
		m_db.DeleteDoc(path, *db_it);
		++db_it;
	}

	//�ݹ������Ŀ¼
}