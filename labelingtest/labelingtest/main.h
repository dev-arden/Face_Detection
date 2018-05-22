#pragma once
#include <math.h>
#include <iostream>
#include<Windows.h>
#include"vfw.h"



#define WIDTH4(width) ((width+3)>>2)<<2 // 폭설정

BITMAPINFO* ChangeBitmapInfo(LONG width, LONG height, const BITMAPINFO* binfo, int i) {

	BITMAPINFO* chbinfo; //24비트 용 비트맵인포 
	BITMAPINFO* blbinfo; //8비트 용 비트맵인포 
	LONG width4;

	if (i == 24) { // 크기를 변환한 bitmapinfo구조체를 리턴해 준다.

		chbinfo = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
		//24비트 정보로 bitmapinfo를 설정
		chbinfo->bmiHeader.biBitCount = 24;
		chbinfo->bmiHeader.biClrImportant = binfo->bmiHeader.biClrImportant;
		chbinfo->bmiHeader.biClrUsed = binfo->bmiHeader.biClrUsed;
		chbinfo->bmiHeader.biCompression = binfo->bmiHeader.biCompression;
		chbinfo->bmiHeader.biHeight = height;
		chbinfo->bmiHeader.biPlanes = binfo->bmiHeader.biPlanes;
		chbinfo->bmiHeader.biSize = binfo->bmiHeader.biSize;
		chbinfo->bmiHeader.biWidth = width;

		width4 = width * 3;     //폭 맞춤
		width4 = ((width4 + 3) >> 2) << 2;

		chbinfo->bmiHeader.biSizeImage = width4 * height;
		chbinfo->bmiHeader.biXPelsPerMeter = binfo->bmiHeader.biXPelsPerMeter;
		chbinfo->bmiHeader.biYPelsPerMeter = binfo->bmiHeader.biYPelsPerMeter;
		return  chbinfo;

	}
	else if (i == 8)
	{
		blbinfo = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
		//8비트 정보로 bitmapinfo를설정 
		blbinfo->bmiHeader.biBitCount = 8;
		blbinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		blbinfo->bmiHeader.biWidth = width;
		blbinfo->bmiHeader.biHeight = height;

		width4 = ((width + 3) >> 2) << 2; //폭설정

		blbinfo->bmiHeader.biXPelsPerMeter = 0;
		blbinfo->bmiHeader.biYPelsPerMeter = 0;
		blbinfo->bmiHeader.biSizeImage = width4*height;
		blbinfo->bmiHeader.biPlanes = 1;
		blbinfo->bmiHeader.biClrUsed = 256;
		blbinfo->bmiHeader.biCompression = 0;
		blbinfo->bmiHeader.biClrImportant = 256;
		for (i = 0; i<256; ++i)
		{
			blbinfo->bmiColors[i].rgbBlue = blbinfo->bmiColors[i].rgbGreen = blbinfo->bmiColors[i].rgbRed = i;
			blbinfo->bmiColors[i].rgbReserved = 0;
		}

		return blbinfo;

	}
	else
	{
		MessageBox(0, "error", "error", 0);

	}
	return 0;

}
BYTE * Bmp24ToBmp8(BITMAPINFO* binfo8, const BITMAPINFO* binfo, const  BYTE *imageData) {

	LONG count = 0;
	int height, width, oldWidth;
	BYTE *Data8bit;
	LONG width4;

	width4 = binfo->bmiHeader.biWidth;
	width4 = WIDTH4(width4);
	oldWidth = binfo->bmiHeader.biWidth * 3;
	oldWidth = WIDTH4(oldWidth);

	binfo8->bmiHeader.biBitCount = 8;              //8비트 흑백
	binfo8->bmiHeader.biPlanes = 1;            //비트 플레인수 항상 1 
	binfo8->bmiHeader.biXPelsPerMeter = binfo->bmiHeader.biXPelsPerMeter;         //가로 해상도 0
	binfo8->bmiHeader.biYPelsPerMeter = binfo->bmiHeader.biYPelsPerMeter;         //세로 해상도 0 
	binfo8->bmiHeader.biHeight = binfo->bmiHeader.biHeight;        //높이
	binfo8->bmiHeader.biWidth = binfo->bmiHeader.biWidth;        //폭 
	binfo8->bmiHeader.biCompression = 0;          //압축하지 않은 데이터
	binfo8->bmiHeader.biSizeImage = width4*binfo->bmiHeader.biHeight;   //영상의 크기 지정 //꼭 체크
	binfo8->bmiHeader.biClrUsed = binfo8->bmiHeader.biClrImportant = 256; //실제 사용 색상수와 실제 중요 색상수는 256으로 설정
	binfo8->bmiHeader.biSize = sizeof(binfo8->bmiHeader);     //이 구조체의 크기

	for (unsigned int i = 0; i<256; i++) {
		binfo8->bmiColors[i].rgbRed = binfo8->bmiColors[i].rgbBlue = binfo8->bmiColors[i].rgbGreen = i;
		binfo8->bmiColors[i].rgbReserved = 0;
	}

	Data8bit = (BYTE*)malloc(binfo8->bmiHeader.biSizeImage);


	for (height = 0; height<binfo8->bmiHeader.biHeight; height++) {

		for (width = 0; width<binfo8->bmiHeader.biWidth; width++) {

			if (height<binfo->bmiHeader.biHeight&&width<binfo->bmiHeader.biWidth) {

				count = (imageData[(height*oldWidth) + width * 3 + 0] +
					imageData[(height*oldWidth) + width * 3 + 1] +
					imageData[(height*oldWidth) + width * 3 + 2]) / 3;

				Data8bit[(height*width4) + width] = count;
			}
			else
				Data8bit[(height*width4) + width] = 0;

		}
	}
	return Data8bit;
}
//선형보간으로 이미지 크기 변경 
BYTE * Bmp24ToYUV(BITMAPINFO* binfo8, const BITMAPINFO* binfo, const  BYTE *imageData) {

	LONG count = 0;
	int height, width, oldWidth;
	BYTE *Data8bit;
	LONG width4;

	width4 = binfo->bmiHeader.biWidth;
	width4 = WIDTH4(width4);
	oldWidth = binfo->bmiHeader.biWidth * 3;
	oldWidth = WIDTH4(oldWidth);

	binfo8->bmiHeader.biBitCount = 8;              //8비트 흑백
	binfo8->bmiHeader.biPlanes = 1;            //비트 플레인수 항상 1 
	binfo8->bmiHeader.biXPelsPerMeter = binfo->bmiHeader.biXPelsPerMeter;         //가로 해상도 0
	binfo8->bmiHeader.biYPelsPerMeter = binfo->bmiHeader.biYPelsPerMeter;         //세로 해상도 0 
	binfo8->bmiHeader.biHeight = binfo->bmiHeader.biHeight;        //높이
	binfo8->bmiHeader.biWidth = binfo->bmiHeader.biWidth;        //폭 
	binfo8->bmiHeader.biCompression = 0;          //압축하지 않은 데이터
	binfo8->bmiHeader.biSizeImage = width4*binfo->bmiHeader.biHeight;   //영상의 크기 지정 //꼭 체크
	binfo8->bmiHeader.biClrUsed = binfo8->bmiHeader.biClrImportant = 256; //실제 사용 색상수와 실제 중요 색상수는 256으로 설정
	binfo8->bmiHeader.biSize = sizeof(binfo8->bmiHeader);     //이 구조체의 크기

	for (unsigned int i = 0; i<256; i++) {
		binfo8->bmiColors[i].rgbRed = binfo8->bmiColors[i].rgbBlue = binfo8->bmiColors[i].rgbGreen = i;
		binfo8->bmiColors[i].rgbReserved = 0;
	}

	Data8bit = (BYTE*)malloc(binfo8->bmiHeader.biSizeImage);

	unsigned char R, G, B;

	for (height = 0; height<binfo8->bmiHeader.biHeight; height++) {

		for (width = 0; width<binfo8->bmiHeader.biWidth; width++) {

			if (height<binfo->bmiHeader.biHeight&&width<binfo->bmiHeader.biWidth) {

				B = imageData[(height*oldWidth) + width * 3 + 0];
				G = imageData[(height*oldWidth) + width * 3 + 1];
				R = imageData[(height*oldWidth) + width * 3 + 2];

				count = (0.299*R + 0.587*G + 0.114*B +
					-0.16874*R - 0.3313*G + 0.500*B + 128 +
					0.500*R - 0.4187*G - 0.0813*B + 128) / 3;
				Data8bit[(height*width4) + width] = count;
			}
			else
				Data8bit[(height*width4) + width] = 0;

		}
	}
	return Data8bit;
}
BYTE* ReSizeImage(BITMAPINFO *binfo, const BITMAPINFO* cbinfo, const BYTE* Data, int i)
{
	BYTE newValue, R, G, B; //데이터값 
	BYTE* chData; //실제 데이터 넣을 포인터 
	int new_height = binfo->bmiHeader.biHeight;//새로운 이미지의 높이 계산 
	int new_width = binfo->bmiHeader.biWidth;//새로운 이미지의 폭 계산 
	int heightm1 = cbinfo->bmiHeader.biHeight - 1;
	int widthm1 = cbinfo->bmiHeader.biWidth - 1;
	int where;//,org_where;
	int r, c;//타겟 이미지 좌표 
	float r_orgr, r_orgc;//원 이미지 상의 해당 좌표 (실수값)
	int i_orgr, i_orgc;//원 이미지 상의 해당 좌표 (정수값)
	float sr, sc;// 예 1.24-1=0.24
	float I1, I2, I3, I4; //선형보간으로 얻는 값들 
	float zoominfactor = (float)binfo->bmiHeader.biWidth / cbinfo->bmiHeader.biWidth;
	unsigned int count = 0;

	//Zoom Image를 위한 동적 메모리 할당
	// 원본이미지 폭 
	long width4 = binfo->bmiHeader.biWidth * 3;
	width4 = WIDTH4(width4);

	// 수정되는 이미 폭 
	long cwidth4 = cbinfo->bmiHeader.biWidth * 3;
	cwidth4 = WIDTH4(cwidth4);

	//동적할당 
	chData = (BYTE*)malloc(binfo->bmiHeader.biSizeImage);

	//흑백일경우 
	if (i == 8)
	{
		for (r = 0; r<new_height; r++)
		{
			r = r;
			for (c = 0; c<new_width; c++)
			{
				r_orgr = r / zoominfactor;
				r_orgc = c / zoominfactor;
				i_orgr = floor(r_orgr);//예: floor(2.8)=2.0
				i_orgc = floor(r_orgc);
				sr = r_orgr - i_orgr;
				sc = r_orgc - i_orgc;
				//범위 조사 
				//원이미지의 범위를 벗어나는 경우 0값 할당 
				if (i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
				{
					where = r*width4 / 3 + c;
					chData[where] = 0;
					//	chData[where+1]=0;
					//	chData[where+2]=0;

				}
				//원 이미지의 범위 내에 존재 이중 선형 보간 수행 
				else
				{
					count++;
					where = r*width4 / 3 + c;


					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 0]; //(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 0];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 0];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 0];//(org_r+1,org_c)

																					//이중 선형 보간을 통한 새로운 밝기값 계산
					R = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					// newValue = (I1+I2+I3+I4)/4;
					//  chData[where]=newValue;

					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 1];//(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 1];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 1];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 1];//(org_r+1,org_c)

																					//이중 선형 보간을 통한 새로운 밝기값 계산
					G = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					//newValue = (I1+I2+I3+I4)/4;
					//chData[where+1]=newValue;


					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 2];//(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 2];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 2];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 2];//(org_r+1,org_c)

																					//이중 선형 보간을 통한 새로운 밝기값 계산
					B = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					//newValue = (I1+I2+I3+I4)/4;  //이중 선형 보간을 안할시 test 코드
					//chData[where+2]=newValue; 
					chData[where] = (R + G + B) / 3;
				}
			}
		}
	}
	//24비트 칼라일경우 
	else if (i == 24)
	{
		for (r = 0; r<new_height; r++)
		{
			r = r;
			for (c = 0; c<new_width; c++)
			{
				r_orgr = r / zoominfactor;
				r_orgc = c / zoominfactor;
				i_orgr = floor(r_orgr);//예: floor(2.8)=2.0
				i_orgc = floor(r_orgc);
				sr = r_orgr - i_orgr;
				sc = r_orgc - i_orgc;
				//범위 조사 
				//원이미지의 범위를 벗어나는 경우 0값 할당 
				if (i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
				{
					where = r*width4 + c * 3;
					chData[where] = 0;
					chData[where + 1] = 0;
					chData[where + 2] = 0;
				}
				//원 이미지의 범위 내에 존재 이중 선형 보간 수행 
				else
				{
					count++;
					where = r*width4 + c * 3;

					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 0]; //(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 0];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 0];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 0];//(org_r+1,org_c)
																					//이중 선형 보간을 통한 새로운 밝기값 계산
					newValue = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					// newValue = (I1+I2+I3+I4)/4;
					chData[where] = newValue;

					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 1];//(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 1];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 1];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 1];//(org_r+1,org_c)

																					//이중 선형 보간을 통한 새로운 밝기값 계산
					newValue = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					//newValue = (I1+I2+I3+I4)/4;
					chData[where + 1] = newValue;


					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 2];//(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 2];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 2];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 2];//(org_r+1,org_c)

																					//이중 선형 보간을 통한 새로운 밝기값 계산
					newValue = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					//newValue = (I1+I2+I3+I4)/4;  //이중 선형 보간을 안할시 test 코드
					chData[where + 2] = newValue;
				}
			}
		}
	}
	return chData;
}

int SaveBmp8bit(const BITMAPINFO* binfo8, const BYTE *Data8Bit) {

	FILE *file;
	BITMAPFILEHEADER bfh8;  //흑백영상을 위한 파일 헤더를 많듬 
	bfh8.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	bfh8.bfReserved1 = 0;
	bfh8.bfReserved2 = 0;
	bfh8.bfSize = bfh8.bfOffBits + binfo8->bmiHeader.biSizeImage;
	bfh8.bfType = 0x4d42;

	file = fopen("8bit.bmp", "wb");   //흑백영상을 bmp파일로 저장
	if (file == NULL) {
		return -1;
	}

	fwrite(&bfh8, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&(binfo8->bmiHeader), sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(&(binfo8->bmiColors), sizeof(RGBQUAD), 256, file);
	fwrite(Data8Bit, sizeof(BYTE), binfo8->bmiHeader.biSizeImage, file);
	MessageBox(0, "흑백 스크린샷 완료", "시작", 0);
	fclose(file);

	return 0;

}

BYTE* ReadBmp(char *filename, BITMAPINFO* binfo) {

	FILE *file = fopen(filename, "rb");
	LONG Width;
	BYTE *imageData;
	BITMAPFILEHEADER bmfh;

	if (file == NULL)
		return 0;

	//비트맵 파일 헤더 읽음 
	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, file);
	//비트맵 인포 헤더를 읽음
	fread(&(binfo->bmiHeader), sizeof(BITMAPINFOHEADER), 1, file);

	if (binfo->bmiHeader.biBitCount == 24) {
		Width = binfo->bmiHeader.biWidth * 3;
		Width = WIDTH4(Width);

		binfo->bmiHeader.biSizeImage = Width * binfo->bmiHeader.biHeight;
		//이미지 사이즈가 0 으로 된 경우가 있어서 직접넣어준다.

		imageData = (BYTE*)malloc(binfo->bmiHeader.biSizeImage);
		//이미지 데이터를 읽음
		fread(imageData, sizeof(BYTE), binfo->bmiHeader.biSizeImage, file);
	}

	fclose(file);

	return imageData;
}


BYTE* MakeHistoBMP(BITMAPINFO* binfo, int *buf, BYTE* imageData, const BITMAPINFO Bm) {

	BYTE* cpyData;
	int iNum;
	int height, width;
	LONG width4;
	LONG width5;

	height = Bm.bmiHeader.biHeight;  //캡쳐영상의 세로
	width = Bm.bmiHeader.biWidth;  //캡쳐영상의 가로

	width5 = binfo->bmiHeader.biWidth;
	width5 = WIDTH4(width5);
	width4 = Bm.bmiHeader.biWidth * 3;
	width4 = WIDTH4(width4);
	for (height = 0; height <Bm.bmiHeader.biHeight; height++)
	{
		for (width = 0; width < Bm.bmiHeader.biWidth; width++)
		{
			iNum = (imageData[(height*width4) + width * 3 + 0] + imageData[(height*width4) + width * 3 + 1] + imageData[(height*width4) + width * 3 + 2]) / 3;
			buf[iNum]++;
		}
	}
	cpyData = (BYTE *)malloc(binfo->bmiHeader.biSizeImage);
	for (iNum = 0; iNum <256; iNum++)
	{
		buf[iNum] = buf[iNum] / 3;
	}
	iNum = 0;
	for (width = 256; width >= 0; width--)
	{
		for (height = 0; height < binfo->bmiHeader.biHeight; height++)
		{
			if (buf[iNum] == 0)
			{
				cpyData[(height*width5) + width] = 255;
				buf[iNum] = 0;

			}
			else
			{
				cpyData[(height*width5) + width] = 0;
				buf[iNum]--;
			}

		}
		iNum++;
	}
	return cpyData;
}

BYTE* Make2JIN(BITMAPINFO* binfo, BYTE* imageData, const BITMAPINFO Bm)
{
	BYTE* cpyData;
	int iNum;
	int height, width;
	LONG width4;
	LONG width5;
	width5 = binfo->bmiHeader.biWidth;
	width5 = WIDTH4(width5);
	width4 = Bm.bmiHeader.biWidth * 3;
	width4 = WIDTH4(width4);
	cpyData = (BYTE *)malloc(binfo->bmiHeader.biSizeImage);

	for (height = 0; height <Bm.bmiHeader.biHeight; height++)
	{
		for (width = 0; width < Bm.bmiHeader.biWidth; width++)
		{
			iNum = (imageData[(height*width4) + width * 3 + 0] + imageData[(height*width4) + width * 3 + 1] +
				imageData[(height*width4) + width * 3 + 2]) / 3;
			if (iNum > 128)
			{
				cpyData[(height*width5) + width] = 255;
			}
			else
			{
				cpyData[(height*width5) + width] = 0;
			}

		}
	}
	return cpyData;
}



