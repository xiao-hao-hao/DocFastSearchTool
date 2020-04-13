#include "Sysutil.h"

void DirectionList(const string &path, vector<string> &subfile, vector<string> &subdir)
{
	string _path = path;
	_path += "\\*.*";
	_finddata_t fileinfo;
	long handle = _findfirst(_path.c_str(), &fileinfo);//����path·���µĵ�һ���ļ�
	if (handle == -1)//����ʧ��
	{
		perror("_findfirst");
		return;
	}
	
	do
	{
		if (strcmp(fileinfo.name, ".")==0 || strcmp(fileinfo.name, "..")==0)
			continue;
		if (fileinfo.attrib & _A_SUBDIR)//�ж���һ��Ŀ¼���ݹ����
		{
			//cout << fileinfo.name << endl;
			//string tmp_path = path;
			//tmp_path += "\\";
			//tmp_path += fileinfo.name;//�µ�����·��
			//DirectionList(tmp_path);
			subdir.push_back(fileinfo.name);//������������Ŀ¼�ŵ�subdir��
		}
		else
		{
			//cout << fileinfo.name << endl;
			subfile.push_back(fileinfo.name);//�������������ļ��ŵ�subfile��
		}
	} while(_findnext(handle, &fileinfo) == 0);

	_findclose(handle);
}