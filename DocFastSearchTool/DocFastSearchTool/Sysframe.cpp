#include "Sysframe.h"

#define WIDTH 120
#define HEIGHT 30

void SetCurPos(int x, int y)//���ù���λ�ã�x���У����������꣬y���У����������
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//��ñ�׼����ľ��
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
	//���ñ���
	char buffer[MAX_TITLE_SIZE+5+1+1] = "title ";//title=5 space=1 \0=1
	strcat(buffer, title);
	system(buffer);
	
	//���ÿͻ��˵Ŀ��
	char mode[128] = {0};
	sprintf(mode, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
	system(mode);

	//������ɫ
	system("color E4");

	//���ƽ�����
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
	//�ڿ�������������
}

