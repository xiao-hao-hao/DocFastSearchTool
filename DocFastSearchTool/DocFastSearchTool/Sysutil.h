#pragma once

#include "Common.h"

void DirectoryList(const string &path, vector<string> &subfile, vector<string> &subdir);


/////////////////////////////////////////////////////////////////////////////////////
//��־
//��ȷ��ʱ��Ͳ���ӡ��־��
#ifndef __TRACE__  //��ȷʱ����־
//#define __TRACE__
#endif

#ifndef __DEBUG__  //����ʱ����־
#define __DEBUG__
#endif

///////////////////////////////////////////////////////////////////////
static std::string GetFileName(const std::string& path)//��·���л�ȡ�ļ���
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

//���ڵ���׷�ݵ�trace log
inline static void __TraceDebug(const char* filename, int line, const char* function, const char* format, ...)
{
#ifdef __TRACE__
	//������ú�������Ϣ
	fprintf(stdout, "[TRACE][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);

	//����û����trace��Ϣ
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
	//������ú�������Ϣ
	fprintf(stdout, "[ERROR][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);

	//����û����trace��Ϣ
	va_list args; //char*����  ����һ��������ת�������б�
	//va_start�꣬��ȡ�ɱ�����б�ĵ�һ�������ĵ�ַ
	va_start(args, format);//��ʼ������
	//������printf��ֻ�ǲ����Լ�д��ʽ����һ��format�Ϳ�����
	vfprintf(stdout, format, args);//ʹ�ò����б��͸�ʽ���������׼���
	//va_end�꣬���va_list�ɱ�����б�
	va_end(args);//���������б���va_start�ɶ�ʹ��

	fprintf(stdout, " errmsg:%s, errno:%d\n", strerror(errno), errno);
#endif
}
//__VA_ARGS__ ��һ���ɱ�����ĺ�(gcc֧��)��ʵ��˼����Ǻ궨���в����б�����һ������Ϊʡ�Ժţ�Ҳ���������㣩��
//����Ԥ�����_ _VA_ARGS_ _�Ϳ��Ա������滻�����У��滻ʡ�Ժ���������ַ�������##����֧��0���ɱ�����������
//��printf
#define TRACE_LOG(...) \
	__TraceDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

#define ERROR_LOG(...) \
	__ErrorDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

////////////////////////////////////////////////////////////
//�м��߼���
string ChineseConvertPinYinAllSpell(const std::string& dest_chinese);
std::string ChineseConvertPinYinInitials(const std::string& name);
void ColorPrintf(const char* str);