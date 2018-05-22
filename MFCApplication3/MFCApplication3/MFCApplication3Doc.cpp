
// MFCApplication3Doc.cpp : CMFCApplication3Doc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFCApplication3.h"
#endif

#include "MFCApplication3Doc.h"

#include <propkey.h>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCApplication3Doc

IMPLEMENT_DYNCREATE(CMFCApplication3Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFCApplication3Doc, CDocument)
END_MESSAGE_MAP()


// CMFCApplication3Doc ����/�Ҹ�

CMFCApplication3Doc::CMFCApplication3Doc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CMFCApplication3Doc::~CMFCApplication3Doc()
{
}

BOOL CMFCApplication3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	::OpenClipboard(NULL);
	if (!IsClipboardFormatAvailable(CF_DIB)) return FALSE;
	HGLOBAL m_hImage = ::GetClipboardData(CF_DIB);
	::CloseClipboard();

	LPSTR pDLB = (LPSTR)::GlobalLock((HGLOBAL)m_hImage);

	memcpy(&dibHi, pDLB, sizeof(BITMAPINFOHEADER));
	height = dibHi.biHeight;
	width = dibHi.biWidth;

	int rwsize = WIDTHBYTES(dibHi.biBitCount*width);
	DWORD dwBitsSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + rwsize*height * sizeof(char);
	m_InImg = new unsigned char[dibHi.biSizeImage];
	m_OutImg = new unsigned char[dibHi.biSizeImage];

	if (dibHi.biBitCount == 8)
	{
		memcpy(palRGB, pDLB + sizeof(BITMAPINFOHEADER), sizeof(RGBQUAD) * 256);
		memcpy(m_InImg, pDLB + dwBitsSize - dibHi.biSizeImage, dibHi.biSizeImage);
	}
	else
		memcpy(m_InImg, pDLB + sizeof(BITMAPINFOHEADER), dibHi.biSizeImage);

	dibHf.bfType = 0x4d42;
	dibHf.bfSize = dwBitsSize + sizeof(BITMAPFILEHEADER);
	if (dibHi.biBitCount == 24)
		dibHf.bfSize -= sizeof(RGBQUAD) * 256;
	dibHf.bfOffBits = dibHf.bfSize - rwsize*height * sizeof(char);
	dibHf.bfReserved1 = dibHf.bfReserved2 = 0;


	return TRUE;

}




// CMFCApplication3Doc serialization

void CMFCApplication3Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CMFCApplication3Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CMFCApplication3Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCApplication3Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCApplication3Doc ����

#ifdef _DEBUG
void CMFCApplication3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCApplication3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCApplication3Doc ���


BOOL CMFCApplication3Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	CFile hFile;
	hFile.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);
	hFile.Read(&dibHf, sizeof(BITMAPFILEHEADER));
	if (dibHf.bfType != 0x4D42)
	{
		AfxMessageBox(L"Not BMP file!!");
		return FALSE;
	}
	hFile.Read(&dibHi, sizeof(BITMAPINFOHEADER));
	if (dibHi.biBitCount != 8 && dibHi.biBitCount != 24)
	{
		AfxMessageBox(L"Gray/True Color Possible");
		return FALSE;
	}
	if (dibHi.biBitCount == 8)
		hFile.Read(palRGB, sizeof(RGBQUAD) * 256);

	int ImgSize;
	if (dibHi.biBitCount == 8)
	{
		ImgSize = hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - 256 * sizeof(RGBQUAD);
	}
	else if (dibHi.biBitCount == 24)
	{
		ImgSize = hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	}
	m_InImg = new unsigned char[ImgSize];
	m_OutImg = new unsigned char[ImgSize];

	hFile.Read(m_InImg, ImgSize);
	hFile.Close();
	height = dibHi.biHeight;
	width = dibHi.biWidth;

	if (dibHi.biBitCount == 24)
		return TRUE;

	int i, j, index;
	int rwsize = WIDTHBYTES(dibHi.biBitCount*width);
	for (i = 0; 0 < height; i++)
	{
		index = i*rwsize;
		for (j = 0; j < width; j++) {
			m_InImg[index + j] = (unsigned char)palRGB[(int)m_InImg[index + j]].rgbBlue;
		}
	}

	ImgType = 0;
	if (dibHi.biBitCount == 8)
		ImgType = 1;
	return TRUE;
}


void CMFCApplication3Doc::CopyClipboard(BYTE* m_CpyImg, int height, int width, int biBitCount)
{
	int rwsize = WIDTHBYTES(biBitCount*width);
	DWORD dwBitsSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + rwsize*height * sizeof(char);

	HGLOBAL m_hImage = (HGLOBAL)::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	LPSTR pDLB = (LPSTR)::GlobalLock((HGLOBAL)m_hImage);

	BITMAPINFOHEADER dibCpyHi;
	memcpy(&dibCpyHi, &dibHi, sizeof(BITMAPINFOHEADER));
	dibCpyHi.biBitCount = biBitCount;
	dibCpyHi.biHeight = height;
	dibCpyHi.biWidth = width;
	dibCpyHi.biSizeImage = height*rwsize;
	if (biBitCount == 8)
		dibCpyHi.biClrUsed = dibCpyHi.biClrImportant = 256;
	memcpy(pDLB, &dibCpyHi, sizeof(BITMAPINFOHEADER));
	if (biBitCount == 8)
	{
		memcpy(pDLB + sizeof(BITMAPINFOHEADER), palRGB, sizeof(RGBQUAD) * 256);
		memcpy(pDLB + dwBitsSize - dibCpyHi.biSizeImage, m_CpyImg, dibCpyHi.biSizeImage);
	}
	else
		memcpy(pDLB + sizeof(BITMAPINFOHEADER), m_CpyImg, dibCpyHi.biSizeImage);

	::OpenClipboard(NULL);
	::SetClipboardData(CF_DIB, m_hImage);
	::CloseClipboard();

	::GlobalUnlock((HGLOBAL)m_hImage);
	GlobalFree(m_hImage);
}


void CMFCApplication3Doc::Onskin()
{
	int i, j, num_C, pixR, pixG, pixB;
	int b, g, r;
	int y, cb, cr;
	unsigned char *m_temp;
	int MAX_x, MAX_y, MIN_x, MIN_y;
	int Fx, Fy, Dx, Dy;

	if (dibHi.biBitCount == 24)
		num_C = 3;
	else if (dibHi.biBitCount == 8)
		num_C = 1;

	m_height = dibHi.biHeight;
	m_width = dibHi.biWidth;
	m_size = m_height*m_width;
	m_OutputImage = new BYTE[m_size * 3];
	m_temp = new unsigned char[m_size];
	m_Test = new BYTE[m_size * 3];

	if (dibHi.biBitCount == 24) {
		for (i = 0; i < m_size; i++) {
			b = m_InImg[i*num_C];// B
			g = m_InImg[i*num_C + 1]; //G
			r = m_InImg[i*num_C + 2]; //R

			y = 0.257*r + 0.504*g + 0.098*b + 16;
			cb = -0.148*r - 0.291*g + 0.439*b + 128;
			cr = 0.439*r - 0.368*g - 0.071*b + 128;

			if ((cr <= 173) && (cr >= 133) && (cb <= 127) && (cb >= 77))
			{
				m_OutputImage[i*num_C] = 255;
				m_OutputImage[i*num_C + 1] = 255;
				m_OutputImage[i*num_C + 2] = 255;
				m_temp[i] = 255;
			}
			else
			{
				m_OutputImage[i*num_C] = 0;
				m_OutputImage[i*num_C + 1] = 0;
				m_OutputImage[i*num_C + 2] = 0;
				m_temp[i] = 0;
			}
		}
	}
	unsigned char* inputImage = new unsigned char[m_height * m_width];
	int * resultImage = new int[m_height * m_width];

	struct labeling
	{
		labeling(int Sx, int Sy) {

			Lx = Sx;
			Ly = Sy;

		}
		int Lx;
		int Ly;

	};

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			// image copy
			inputImage[m_width * y + x] = m_temp[m_width * y + x];
			// initialize result image
			resultImage[m_width * y + x] = 0;
		}
	}

	std::vector<labeling> st;
	std::vector<labeling> test01;
	int labelNumber = 100;
	for (int y = 1; y < m_height - 1; y++) {
		for (int x = 1; x < m_width - 1; x++) {
			if (inputImage[m_width * y + x] != 255 || resultImage[m_width * y + x] != 0) continue;
			labelNumber++;
			st.push_back(labeling(x, y));
			test01.push_back(labeling(x, y));

			while (!st.empty()) {
				int ky = st.back().Ly;
				int kx = st.back().Lx;


				MAX_x = st.front().Lx;
				MIN_x = st.back().Lx;
				MAX_y = st.front().Ly;
				MIN_y = st.back().Ly;
				Fx = labelMax(kx, MAX_x);
				Fy = labelMax(ky, MAX_y);
				Dx = labelMin(kx, MIN_x);
				Dy = labelMin(ky, MIN_y);
				//      std::cout << "ó��X��ǥ" << MAX_x << std::endl;
				//      std::cout << "ó��Y��ǥ" << MAX_y << std::endl;
				//std::cout << "X��ǥ"<< kx << std::endl;
				st.pop_back();

				resultImage[width * ky + kx] = labelNumber;
				for (int ny = ky - 1; ny <= ky + 1; ny++) {
					// y�� ������ ����� �� ����
					if (ny < 0 || ny >= height) continue;
					for (int nx = kx - 1; nx <= kx + 1; nx++) {
						// x�� ������ ����� �� ����
						if (nx < 0 || nx >= width) continue;
						// source image�� ���� �ְ� labeling�� �ȵ� ��ǥ�� stack�� push
						if (inputImage[width * ny + nx] != 255 || resultImage[width * ny + nx] != 0) continue;
						st.push_back(labeling(nx, ny));
						test01.push_back(labeling(nx, ny));
						//std::cout << "�󺧸� �� �� ��ǥX" << nx << std::endl;
						//std::cout << "�󺧸� �� �� ��ǥY" << ny << std::endl;
						resultImage[width * ny + nx] = labelNumber;

					}
				}
			}
		}
	}

	MAX_x = test01[0].Lx;
	MAX_y = test01[0].Ly;
	MIN_x = test01[0].Lx;
	MIN_y = test01[0].Ly;

	for (int i = 0; i < test01.size(); i++) {
		int a = test01[i].Lx;
		int b = test01[i].Ly;
		MAX_x = labelMax(MAX_x, a);
		MAX_y = labelMax(MAX_y, b);
		MIN_x = labelMin(MIN_x, a);
		MIN_y = labelMin(MIN_y, b);
	}

	std::cout << "�ִ�X��ǥ" << MAX_x << std::endl;
	std::cout << "�ִ�Y��ǥ" << MAX_y << std::endl;
	std::cout << "�ּ�X��ǥ" << MIN_x << std::endl;
	std::cout << "�ּ�Y��ǥ" << MIN_y << std::endl;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			m_OutImg[(width * y + x) * 3] = resultImage[width * y + x];
			m_OutImg[(width * y + x) * 3 + 1] = resultImage[width * y + x];
			m_OutImg[(width * y + x) * 3 + 2] = resultImage[width * y + x];
		}
	}
	
	
	
	for (int x = MIN_x; x < MAX_x; x++) {
		m_OutImg[(width * MAX_y + x) * 3] = 0;
		m_OutImg[(width * MAX_y + x) * 3 + 1] = 0;
		m_OutImg[(width * MAX_y + x) * 3 + 2] = 255;
	}
	
	/*
	for (int x = MIN_x; x < MAX_x; x++) {
		m_OutImg[(width * MIN_y + x) * 3] = 0;
		m_OutImg[(width * MIN_y + x) * 3 + 1] = 0;
		m_OutImg[(width * MIN_y + x) * 3 + 2] = 255;
	}
	*/
	cout << MAX_x - MIN_x << ","<< MAX_y - MIN_y <<endl;
	int vertical = MAX_x - MIN_x;
	int horizon = 0.3 * vertical;
	int real = MIN_y + horizon;

	for (int x = MIN_x; x < MAX_x; x++) {
		m_OutImg[(width *real + x) * 3] = 0;
		m_OutImg[(width * real + x) * 3 + 1] = 0;
		m_OutImg[(width * real + x) * 3 + 2] = 255;
	}
	for (int y = horizon; y < MAX_y; y++) {
		m_OutImg[(width * y + MAX_x) * 3] = 0;
		m_OutImg[(width * y + MAX_x) * 3 + 1] = 0;
		m_OutImg[(width * y + MAX_x) * 3 + 2] = 255;
	}
	for (int y = horizon; y < MAX_y; y++) {
		m_OutImg[(width * y + MIN_x) * 3] = 0;
		m_OutImg[(width * y + MIN_x) * 3 + 1] = 0;
		m_OutImg[(width * y + MIN_x) * 3 + 2] = 255;
	}



	delete[] inputImage;
	delete[] resultImage;
}


int CMFCApplication3Doc::labelMax(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}


int CMFCApplication3Doc::labelMin(int a, int b)
{
	if (a > b)
		return b;
	else
		return a;
}
