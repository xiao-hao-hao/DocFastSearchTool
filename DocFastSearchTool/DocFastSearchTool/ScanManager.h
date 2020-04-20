#pragma once
#include "Common.h"
#include "DataManager.h"

class ScanManager
{
public:
	static ScanManager& CreateInstance(const string &path);
public:
	void StartScan(const string &path);
	void ScanDirectory(const string &path);
private:
	ScanManager();
private:
	//DataManager m_db;
};