#pragma once

#include "Common.h"

void DirectionList(const string &path, vector<string> &subfile, vector<string> &subdir);


/////////////////////////////////////////////////////////////////////////////////////
//日志
//正确的时候就不打印日志了
#ifndef __TRACE__
//#define __TRACE__
#endif

#ifndef __DEBUG__
#define __DEBUG__
#endif

///////////////////////////////////////////////////////////////////////
static std::string GetFileName(const std::string& path)
{
	char ch = '/';

#ifdef _WIN32
	ch = '\\';
#endif
	size_t pos = path.rfind(ch);
	if (pos == std::string::npos)
		return path;
	else
		return path.substr(pos + 1);
}

//用于调试追溯的trace log
inline static void __TraceDebug(const char* filename, int line, const char* function, const char* format, ...)
{
#ifdef __TRACE__
	//输出调用函数的信息
	fprintf(stdout, "[TRACE][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);

	//输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);

	fprintf(stdout, "\n");
#endif
}

inline static void __ErrorDebug(const char* filename, int line, const char* function, const char* format, ...)
{
#ifdef __DEBUG__
	//输出调用函数的信息
	fprintf(stdout, "[ERROR][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);

	//输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);

	fprintf(stdout, " errmsg:%s, errno:%d\n", strerror(errno), errno);
#endif
}

#define TRACE_LOG(...) \
	__TraceDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

#define ERROR_LOG(...) \
	__ErrorDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

////////////////////////////////////////////////////////////
//中间逻辑层
string ChineseConvertPinYinAllSpell(const std::string& dest_chinese);
std::string ChineseConvertPinYinInitials(const std::string& name);