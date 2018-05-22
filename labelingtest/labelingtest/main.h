#pragma once
#include <math.h>
#include <iostream>
#include<Windows.h>
#include"vfw.h"



#define WIDTH4(width) ((width+3)>>2)<<2 // ������

BITMAPINFO* ChangeBitmapInfo(LONG width, LONG height, const BITMAPINFO* binfo, int i) {

	BITMAPINFO* chbinfo; //24��Ʈ �� ��Ʈ������ 
	BITMAPINFO* blbinfo; //8��Ʈ �� ��Ʈ������ 
	LONG width4;

	if (i == 24) { // ũ�⸦ ��ȯ�� bitmapinfo����ü�� ������ �ش�.

		chbinfo = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
		//24��Ʈ ������ bitmapinfo�� ����
		chbinfo->bmiHeader.biBitCount = 24;
		chbinfo->bmiHeader.biClrImportant = binfo->bmiHeader.biClrImportant;
		chbinfo->bmiHeader.biClrUsed = binfo->bmiHeader.biClrUsed;
		chbinfo->bmiHeader.biCompression = binfo->bmiHeader.biCompression;
		chbinfo->bmiHeader.biHeight = height;
		chbinfo->bmiHeader.biPlanes = binfo->bmiHeader.biPlanes;
		chbinfo->bmiHeader.biSize = binfo->bmiHeader.biSize;
		chbinfo->bmiHeader.biWidth = width;

		width4 = width * 3;     //�� ����
		width4 = ((width4 + 3) >> 2) << 2;

		chbinfo->bmiHeader.biSizeImage = width4 * height;
		chbinfo->bmiHeader.biXPelsPerMeter = binfo->bmiHeader.biXPelsPerMeter;
		chbinfo->bmiHeader.biYPelsPerMeter = binfo->bmiHeader.biYPelsPerMeter;
		return  chbinfo;

	}
	else if (i == 8)
	{
		blbinfo = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
		//8��Ʈ ������ bitmapinfo������ 
		blbinfo->bmiHeader.biBitCount = 8;
		blbinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		blbinfo->bmiHeader.biWidth = width;
		blbinfo->bmiHeader.biHeight = height;

		width4 = ((width + 3) >> 2) << 2; //������

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

	binfo8->bmiHeader.biBitCount = 8;              //8��Ʈ ���
	binfo8->bmiHeader.biPlanes = 1;            //��Ʈ �÷��μ� �׻� 1 
	binfo8->bmiHeader.biXPelsPerMeter = binfo->bmiHeader.biXPelsPerMeter;         //���� �ػ� 0
	binfo8->bmiHeader.biYPelsPerMeter = binfo->bmiHeader.biYPelsPerMeter;         //���� �ػ� 0 
	binfo8->bmiHeader.biHeight = binfo->bmiHeader.biHeight;        //����
	binfo8->bmiHeader.biWidth = binfo->bmiHeader.biWidth;        //�� 
	binfo8->bmiHeader.biCompression = 0;          //�������� ���� ������
	binfo8->bmiHeader.biSizeImage = width4*binfo->bmiHeader.biHeight;   //������ ũ�� ���� //�� üũ
	binfo8->bmiHeader.biClrUsed = binfo8->bmiHeader.biClrImportant = 256; //���� ��� ������� ���� �߿� ������� 256���� ����
	binfo8->bmiHeader.biSize = sizeof(binfo8->bmiHeader);     //�� ����ü�� ũ��

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
//������������ �̹��� ũ�� ���� 
BYTE * Bmp24ToYUV(BITMAPINFO* binfo8, const BITMAPINFO* binfo, const  BYTE *imageData) {

	LONG count = 0;
	int height, width, oldWidth;
	BYTE *Data8bit;
	LONG width4;

	width4 = binfo->bmiHeader.biWidth;
	width4 = WIDTH4(width4);
	oldWidth = binfo->bmiHeader.biWidth * 3;
	oldWidth = WIDTH4(oldWidth);

	binfo8->bmiHeader.biBitCount = 8;              //8��Ʈ ���
	binfo8->bmiHeader.biPlanes = 1;            //��Ʈ �÷��μ� �׻� 1 
	binfo8->bmiHeader.biXPelsPerMeter = binfo->bmiHeader.biXPelsPerMeter;         //���� �ػ� 0
	binfo8->bmiHeader.biYPelsPerMeter = binfo->bmiHeader.biYPelsPerMeter;         //���� �ػ� 0 
	binfo8->bmiHeader.biHeight = binfo->bmiHeader.biHeight;        //����
	binfo8->bmiHeader.biWidth = binfo->bmiHeader.biWidth;        //�� 
	binfo8->bmiHeader.biCompression = 0;          //�������� ���� ������
	binfo8->bmiHeader.biSizeImage = width4*binfo->bmiHeader.biHeight;   //������ ũ�� ���� //�� üũ
	binfo8->bmiHeader.biClrUsed = binfo8->bmiHeader.biClrImportant = 256; //���� ��� ������� ���� �߿� ������� 256���� ����
	binfo8->bmiHeader.biSize = sizeof(binfo8->bmiHeader);     //�� ����ü�� ũ��

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
	BYTE newValue, R, G, B; //�����Ͱ� 
	BYTE* chData; //���� ������ ���� ������ 
	int new_height = binfo->bmiHeader.biHeight;//���ο� �̹����� ���� ��� 
	int new_width = binfo->bmiHeader.biWidth;//���ο� �̹����� �� ��� 
	int heightm1 = cbinfo->bmiHeader.biHeight - 1;
	int widthm1 = cbinfo->bmiHeader.biWidth - 1;
	int where;//,org_where;
	int r, c;//Ÿ�� �̹��� ��ǥ 
	float r_orgr, r_orgc;//�� �̹��� ���� �ش� ��ǥ (�Ǽ���)
	int i_orgr, i_orgc;//�� �̹��� ���� �ش� ��ǥ (������)
	float sr, sc;// �� 1.24-1=0.24
	float I1, I2, I3, I4; //������������ ��� ���� 
	float zoominfactor = (float)binfo->bmiHeader.biWidth / cbinfo->bmiHeader.biWidth;
	unsigned int count = 0;

	//Zoom Image�� ���� ���� �޸� �Ҵ�
	// �����̹��� �� 
	long width4 = binfo->bmiHeader.biWidth * 3;
	width4 = WIDTH4(width4);

	// �����Ǵ� �̹� �� 
	long cwidth4 = cbinfo->bmiHeader.biWidth * 3;
	cwidth4 = WIDTH4(cwidth4);

	//�����Ҵ� 
	chData = (BYTE*)malloc(binfo->bmiHeader.biSizeImage);

	//����ϰ�� 
	if (i == 8)
	{
		for (r = 0; r<new_height; r++)
		{
			r = r;
			for (c = 0; c<new_width; c++)
			{
				r_orgr = r / zoominfactor;
				r_orgc = c / zoominfactor;
				i_orgr = floor(r_orgr);//��: floor(2.8)=2.0
				i_orgc = floor(r_orgc);
				sr = r_orgr - i_orgr;
				sc = r_orgc - i_orgc;
				//���� ���� 
				//���̹����� ������ ����� ��� 0�� �Ҵ� 
				if (i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
				{
					where = r*width4 / 3 + c;
					chData[where] = 0;
					//	chData[where+1]=0;
					//	chData[where+2]=0;

				}
				//�� �̹����� ���� ���� ���� ���� ���� ���� ���� 
				else
				{
					count++;
					where = r*width4 / 3 + c;


					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 0]; //(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 0];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 0];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 0];//(org_r+1,org_c)

																					//���� ���� ������ ���� ���ο� ��Ⱚ ���
					R = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					// newValue = (I1+I2+I3+I4)/4;
					//  chData[where]=newValue;

					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 1];//(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 1];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 1];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 1];//(org_r+1,org_c)

																					//���� ���� ������ ���� ���ο� ��Ⱚ ���
					G = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					//newValue = (I1+I2+I3+I4)/4;
					//chData[where+1]=newValue;


					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 2];//(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 2];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 2];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 2];//(org_r+1,org_c)

																					//���� ���� ������ ���� ���ο� ��Ⱚ ���
					B = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					//newValue = (I1+I2+I3+I4)/4;  //���� ���� ������ ���ҽ� test �ڵ�
					//chData[where+2]=newValue; 
					chData[where] = (R + G + B) / 3;
				}
			}
		}
	}
	//24��Ʈ Į���ϰ�� 
	else if (i == 24)
	{
		for (r = 0; r<new_height; r++)
		{
			r = r;
			for (c = 0; c<new_width; c++)
			{
				r_orgr = r / zoominfactor;
				r_orgc = c / zoominfactor;
				i_orgr = floor(r_orgr);//��: floor(2.8)=2.0
				i_orgc = floor(r_orgc);
				sr = r_orgr - i_orgr;
				sc = r_orgc - i_orgc;
				//���� ���� 
				//���̹����� ������ ����� ��� 0�� �Ҵ� 
				if (i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
				{
					where = r*width4 + c * 3;
					chData[where] = 0;
					chData[where + 1] = 0;
					chData[where + 2] = 0;
				}
				//�� �̹����� ���� ���� ���� ���� ���� ���� ���� 
				else
				{
					count++;
					where = r*width4 + c * 3;

					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 0]; //(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 0];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 0];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 0];//(org_r+1,org_c)
																					//���� ���� ������ ���� ���ο� ��Ⱚ ���
					newValue = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					// newValue = (I1+I2+I3+I4)/4;
					chData[where] = newValue;

					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 1];//(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 1];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 1];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 1];//(org_r+1,org_c)

																					//���� ���� ������ ���� ���ο� ��Ⱚ ���
					newValue = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					//newValue = (I1+I2+I3+I4)/4;
					chData[where + 1] = newValue;


					I1 = (float)Data[(cwidth4*i_orgr) + i_orgc * 3 + 2];//(org_r,org_c)
					I2 = (float)Data[(cwidth4*i_orgr) + (i_orgc + 1) * 3 + 2];//(org_r,org_c+1)
					I3 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 2];//(org_r+1,org_c+1)
					I4 = (float)Data[(cwidth4*(i_orgr + 1)) + (i_orgc + 1) * 3 + 2];//(org_r+1,org_c)

																					//���� ���� ������ ���� ���ο� ��Ⱚ ���
					newValue = (BYTE)(I1*(1 - sc)*(1 - sr) + I2*sc*(1 - sr) + I3*sc*sr + I4*(1 - sc)*sr);
					//newValue = (I1+I2+I3+I4)/4;  //���� ���� ������ ���ҽ� test �ڵ�
					chData[where + 2] = newValue;
				}
			}
		}
	}
	return chData;
}

int SaveBmp8bit(const BITMAPINFO* binfo8, const BYTE *Data8Bit) {

	FILE *file;
	BITMAPFILEHEADER bfh8;  //��鿵���� ���� ���� ����� ���� 
	bfh8.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	bfh8.bfReserved1 = 0;
	bfh8.bfReserved2 = 0;
	bfh8.bfSize = bfh8.bfOffBits + binfo8->bmiHeader.biSizeImage;
	bfh8.bfType = 0x4d42;

	file = fopen("8bit.bmp", "wb");   //��鿵���� bmp���Ϸ� ����
	if (file == NULL) {
		return -1;
	}

	fwrite(&bfh8, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&(binfo8->bmiHeader), sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(&(binfo8->bmiColors), sizeof(RGBQUAD), 256, file);
	fwrite(Data8Bit, sizeof(BYTE), binfo8->bmiHeader.biSizeImage, file);
	MessageBox(0, "��� ��ũ���� �Ϸ�", "����", 0);
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

	//��Ʈ�� ���� ��� ���� 
	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, file);
	//��Ʈ�� ���� ����� ����
	fread(&(binfo->bmiHeader), sizeof(BITMAPINFOHEADER), 1, file);

	if (binfo->bmiHeader.biBitCount == 24) {
		Width = binfo->bmiHeader.biWidth * 3;
		Width = WIDTH4(Width);

		binfo->bmiHeader.biSizeImage = Width * binfo->bmiHeader.biHeight;
		//�̹��� ����� 0 ���� �� ��찡 �־ �����־��ش�.

		imageData = (BYTE*)malloc(binfo->bmiHeader.biSizeImage);
		//�̹��� �����͸� ����
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

	height = Bm.bmiHeader.biHeight;  //ĸ�Ŀ����� ����
	width = Bm.bmiHeader.biWidth;  //ĸ�Ŀ����� ����

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



