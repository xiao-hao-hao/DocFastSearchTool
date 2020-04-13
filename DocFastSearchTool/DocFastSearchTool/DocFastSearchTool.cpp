#include "Common.h"
#include "Sysutil.h"

void Test_DirectionList()
{
	const string &path = "D:\\bishe";
	vector<string> subfile, subdir;
	DirectionList(path, subfile, subdir);
	for (const auto &e : subfile)
		cout << e << endl;
	for (const auto &e : subdir)
		cout << e  << endl;
}

int main()
{
	Test_DirectionList();
	return 0;
}