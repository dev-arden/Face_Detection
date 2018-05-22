#pragma once
#define WHITE 0xffffff
#define BLACK 0x000000
#define RED   0x0000ff
#define GREEN 0x00ff00
#define BLUE  0xff0000

#define DRAW_W  320
#define DRAW_H  240

COLORREF rgbOrignal[DRAW_H][DRAW_W];  // ���� �̹���
COLORREF rgb[DRAW_H][DRAW_W];   // ���͸� �̹���

COLORREF Abrgb[DRAW_H][DRAW_W];	// ���� ���� ����ȭ �̹���
COLORREF RBrgb[DRAW_H][DRAW_W];	// �󺧸� �̹���
COLORREF R_RBrgb[DRAW_H][DRAW_W];	// �󺧸� �̹���
COLORREF G_RBrgb[DRAW_H][DRAW_W];	// �󺧸� �̹���
COLORREF B_RBrgb[DRAW_H][DRAW_W];	// �󺧸� �̹���



int R_Pos[2];  // 0 : x, 1 : y
			   //int G_Pos[2];
			   //int B_Pos[2];

int Bigrb = 0;
int R_Bigrb = 0;
int G_Bigrb = 0;
int B_Bigrb = 0;

typedef struct tagRabel
{
	struct tagRabel* next;
	int RNumber;	//�󺧸� ��ȣ ����
	int RCount;     //�󺧸��� ��
}Rabel;

Rabel *RHead;
Rabel *RTail;


typedef struct tagPos
{
	struct tagPos* next;
	int PosX;
	int PosY;
}Pos;

Pos *PHead;
Pos *PTail;

