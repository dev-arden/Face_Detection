#include "main.h"	
#include "rabelstruct.h"	
void Rabel()
{
	HDC hdc;
	HDC hMemDC;
	HBITMAP OldBitmap;

	HPEN MyPen, OldPen;
	int iCntX, iCntY;
	int Jump;
	int Ry = 0, Rx = 0, Gy = 0, Gx = 0, By = 0, Bx = 0;
	static int PaintY = 0, PaintX = 0;
	static int StartFlag = 0;

	COLORREF rgbOrignal[DRAW_H][DRAW_W];  // 원본 이미지
	COLORREF rgbDraw[DRAW_H][DRAW_W];   // 원본 그리기 이미지
	COLORREF rgb[DRAW_H][DRAW_W];   // 필터링 이미지

	hdc = GetDC(hWndMain);
	hMemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(hMemDC, hBit);

	//------------------------------------------------------------------------------------------
	// 비트맵 데이터 2차원 픽셀 배열로 변환
	Jump = 0;
	for (iCntY = 0; iCntY < Bm.bmiHeader.biHeight; ++iCntY) {
		for (iCntX = 0; iCntX < Bm.bmiHeader.biWidth; ++iCntX) {
			rgbDraw[iCntY][iCntX] = rgbOrignal[iCntY][iCntX] =
				RGB(VideoHdr->lpData[Jump + 2], VideoHdr->lpData[Jump + 1], VideoHdr->lpData[Jump]);
			Jump += 3;
		}
	}
	//------------------------------------------------------------------------------------------
	// 필터링


	//------------------------------------------------------------------------------------------
	// 라벨링 

	// 라벨링 좌표 추출
	RabelColorFilter(rgbOrignal, RED);
	Rx = R_Pos[0];	Ry = R_Pos[1];
	RabelColorFilter(rgbOrignal, BLUE);
	Bx = R_Pos[0];	By = R_Pos[1];


	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	MyPen = CreatePen(PS_SOLID, 5, RGB(128, 128, 128));
	OldPen = (HPEN)SelectObject(hdc, MyPen);

	// 라벨링 포인트 그리기
	for (iCntY = 0; iCntY < Bm.bmiHeader.biHeight; ++iCntY) {
		for (iCntX = 0; iCntX < Bm.bmiHeader.biWidth; ++iCntX) {
			rgb[iCntY][iCntX] = WHITE;
		}
	}
	int i, j;
	for (i = Ry - 2; i <= Ry + 2; i++)
		for (j = Rx - 2; j < Rx + 2; j++)
			rgb[i][j] = RED;
	for (i = By - 2; i <= By + 2; i++)
		for (j = Bx - 2; j < Bx + 2; j++)
			rgb[i][j] = BLUE;

	// 두 포인트의 거리 측정
	if ((0 <= Rx - Bx && Rx - Bx<30) || (0 <= Bx - Rx && Bx - Rx<30))
		if ((0 <= Ry - By && Ry - By<30) || (0 <= By - Ry && By - Ry<30))

			// 선 긋기


			// 변환 이미지 메모리DC에 저장
			for (iCntY = 0; iCntY < Bm.bmiHeader.biHeight; ++iCntY) {
				for (iCntX = 0; iCntX < Bm.bmiHeader.biWidth; ++iCntX) {
					SetPixel(hMemDC,
						iCntX, (Bm.bmiHeader.biHeight - iCntY) - 1, rgb[iCntY][iCntX]);
				}
			}
	width = Bm.bmiHeader.biWidth;
	BitBlt(hdc, width * 2 + 150, 200, Bm.bmiHeader.biWidth, Bm.bmiHeader.biHeight,
		hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, OldBitmap);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
}