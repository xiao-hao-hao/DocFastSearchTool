#include "Sysframe.h"

#define WIDTH 120
#define HEIGHT 30
/*
void SetCurPos(int x, int y)//设置光标的位置，x是行，代表纵坐标，y是列，代表横坐标
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获得标准输出的句柄
	COORD pos = {y, x};
	SetConsoleCursorPosition(handle, pos);
}
void DrawRow(int x, int y)
{
	for (int i = 0; i < WIDTH-4; ++i)
	{
		SetCurPos(x, y+i);
		printf("=");
	}
} 
void DrawCol(int x, int y)
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		SetCurPos(i, y);
		printf("||");
	}
}

void DrawFrame(char *title)
{
	//设置标题
	char buffer[MAX_TITLE_SIZE+5+1+1] = "title ";//title=5 space=1 \0=1
	strcat(buffer, title);
	system(buffer);
	
	//设置客户端的宽度
	char mode[128] = {0};
	sprintf(mode, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
	system(mode);

	//设置颜色
	system("color E4");

	//绘制界面框架
	DrawCol(0, 0);
	DrawCol(0, WIDTH-2);
	DrawRow(0, 2);
	DrawRow(2, 2);
	DrawRow(4, 2);
	DrawRow(HEIGHT - 6, 2);
	DrawRow(HEIGHT - 4, 2);
	DrawRow(HEIGHT-2, 2);
}

void DrawMenu()
{
	//在框架里面填充数据
}
*/

void SetCurPos(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(handle, pos);
}

void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 100, 0 };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor_info);
}

void DrawCol(int x, int y)
{
	int i;
	for (i = 0; i < HEIGHT; ++i)
	{
		SetCurPos(x, y + i);
		printf("||");
	}
}

void DrawRow(int x, int y)
{
	int i;
	for (i = 0; i < WIDTH - 4; ++i)
	{
		SetCurPos(x + i, y);
		printf("=");
	}
}

void DrawFrame(char *title)
{
	char buffer[MAX_TITLE_SIZE + 6 + 1] = "title "; //6:title%20 1:\0
	strcat(buffer, title);
	system(buffer); //设置系统标题
	char mode[128] = { 0 };
	sprintf(mode, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
	system(mode); //设置控制台的长度和宽度
	system("color 0F");//设置颜色
	DrawCol(0, 0);
	DrawCol(WIDTH - 2, 0);
	DrawRow(2, 0);
	DrawRow(2, 2);
	DrawRow(2, 4);
	DrawRow(2, HEIGHT - 4);
	DrawRow(2, HEIGHT - 2);
}

void SystemEnd()
{
	SetCurPos((WIDTH-4-strlen("请按任意键继续. . ."))/2+2, HEIGHT-1);
}

extern char *title;
void DrawMenu()
{
	//根据自己的需求进行实现
	//标题的设置
	SetCurPos((WIDTH - 4 - strlen(title)) / 2, 1);
	printf("%s", title);

	//名称 路径
	SetCurPos(2, 3);
	printf("%-30s %-85s", "名称", "路径");

	//退出设置
	SetCurPos((WIDTH - 4 - strlen("exit 退出系统 .")) / 2, HEIGHT - 3);
	printf("%s", "exit 退出系统 .");
	DrawRow(2, HEIGHT - 6);

	//SetCurPos((WIDTH-4-strlen("请输入:>"))/2, 15);
	SetCurPos(2, HEIGHT - 5);
	printf("%s", "请输入:>");
}

