#include "Common.h"
#include "Sysutil.h"
#include "./sqlite/sqlite3.h"
#include "DataManager.h"
#include "ScanManager.h"

char *title = "文档快速搜索工具";

int main(int argc, char *argv[])
{
	HideCursor();
	const string &path1 = "C:";
	const string &path2 = "D:";
	const string &path3 = "E:";
	//创建扫描实例
	ScanManager::CreateInstance(path1, path2, path3);
	//sm.ScanDirectory(path);

	//创建搜索实例
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



/*
int main()
{
string strlower = "woSHi赵秉宇";
transform(strlower.begin(), strlower.end(), strlower.begin(), ::tolower);
cout << strlower << endl;//woshizhaobingyu
return 0;
}
*/