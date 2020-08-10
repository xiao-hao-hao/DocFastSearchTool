#pragma once
#include "Common.h"
#include "DataManager.h"

class ScanManager
{
public:
	static ScanManager& CreateInstance(const string &path1, const string &path2, const string &path3);
public:
	void StartScan(const string &path);
	void ScanDirectory(const string &path);
private:
	ScanManager();
	ScanManager(const ScanManager&);
	ScanManager& operator=(const ScanManager&);
	//static ScanManager m_scan;
};
//ScanManager ScanManager::m_scan;