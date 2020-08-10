#pragma once

#include "Common.h"

void DirectoryList(const string &path, vector<string> &subfile, vector<string> &subdir);


/////////////////////////////////////////////////////////////////////////////////////
//日志
//正确的时候就不打印日志了
#ifndef __TRACE__  //正确时的日志
//#define __TRACE__
#endif

#ifndef __DEBUG__  //错误时的日志
#define __DEBUG__
#endif

///////////////////////////////////////////////////////////////////////
static std::string GetFileName(const std::string& path)//从路径中获取文件名
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
	va_list args; //char*类型  声明一个变量来转换参数列表
	//va_start宏，获取可变参数列表的第一个参数的地址
	va_start(args, format);//初始化变量
	//类似于printf，只是不用自己写格式，填一个format就可以了
	vfprintf(stdout, format, args);//使用参数列表发送格式化输出到标准输出
	//va_end宏，清空va_list可变参数列表
	va_end(args);//结束变量列表，和va_start成对使用

	fprintf(stdout, " errmsg:%s, errno:%d\n", strerror(errno), errno);
#endif
}
//__VA_ARGS__ 是一个可变参数的宏(gcc支持)。实现思想就是宏定义中参数列表的最后一个参数为省略号（也就是三个点）。
//这样预定义宏_ _VA_ARGS_ _就可以被用在替换部分中，替换省略号所代表的字符串。加##用来支持0个可变参数的情况。
//像printf
#define TRACE_LOG(...) \
	__TraceDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

#define ERROR_LOG(...) \
	__ErrorDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

////////////////////////////////////////////////////////////
//中间逻辑层
string ChineseConvertPinYinAllSpell(const std::string& dest_chinese);
std::string ChineseConvertPinYinInitials(const std::string& name);
void ColorPrintf(const char* str);