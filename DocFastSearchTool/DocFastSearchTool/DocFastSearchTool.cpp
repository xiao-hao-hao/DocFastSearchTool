#include "Common.h"
#include "Sysutil.h"
#include "./sqlite/sqlite3.h"
#include "DataManager.h"
#include "ScanManager.h"

char *title = "�ĵ�������������";

int main(int argc, char *argv[])
{
	HideCursor();
	const string &path1 = "C:";
	const string &path2 = "D:";
	const string &path3 = "E:";
	//����ɨ��ʵ��
	ScanManager::CreateInstance(path1, path2, path3);
	//sm.ScanDirectory(path);

	//��������ʵ��
	DataManager &dm = DataManager::GetInstance();
	vector<pair<string, string> > doc_path;
	string key;
	while (1)
	{
		system("cls");
		DrawFrame(title);
		DrawMenu();
		cin >> key;
		if (key == "exit")
			break;
		dm.Search(key, doc_path);

		int init_row = 5;//�ɽ������
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



/*
int main()
{
string strlower = "woSHi�Ա���";
transform(strlower.begin(), strlower.end(), strlower.begin(), ::tolower);
cout << strlower << endl;//woshizhaobingyu
return 0;
}
*/