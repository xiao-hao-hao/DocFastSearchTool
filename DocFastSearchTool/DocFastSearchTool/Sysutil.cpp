#include "Sysutil.h"

void DirectionList(const string &path, vector<string> &subfile, vector<string> &subdir)
{
	string _path = path;
	_path += "\\*.*";
	_finddata_t fileinfo;
	long handle = _findfirst(_path.c_str(), &fileinfo);//搜索path路径下的第一个文件
	if (handle == -1)//搜索失败
	{
		perror("_findfirst");
		return;
	}
	
	do
	{
		if (strcmp(fileinfo.name, ".")==0 || strcmp(fileinfo.name, "..")==0)
			continue;
		if (fileinfo.attrib & _A_SUBDIR)//判断是一个目录，递归遍历
		{
			//cout << fileinfo.name << endl;
			//string tmp_path = path;
			//tmp_path += "\\";
			//tmp_path += fileinfo.name;//新的搜索路径
			//DirectionList(tmp_path);
			subdir.push_back(fileinfo.name);//将搜索出来的目录放到subdir中
		}
		else
		{
			//cout << fileinfo.name << endl;
			subfile.push_back(fileinfo.name);//把搜索出来的文件放到subfile中
		}
	} while(_findnext(handle, &fileinfo) == 0);

	_findclose(handle);
}