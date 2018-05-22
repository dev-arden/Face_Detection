
// MFCApplication3Doc.cpp : CMFCApplication3Doc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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


// CMFCApplication3Doc 생성/소멸

CMFCApplication3Doc::CMFCApplication3Doc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CMFCApplication3Doc::~CMFCApplication3Doc()
{
}

BOOL CMFCApplication3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

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
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CMFCApplication3Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CMFCApplication3Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CMFCApplication3Doc 진단

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


// CMFCApplication3Doc 명령


BOOL CMFCApplication3Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
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
				//      std::cout << "처음X좌표" << MAX_x << std::endl;
				//      std::cout << "처음Y좌표" << MAX_y << std::endl;
				//std::cout << "X좌표"<< kx << std::endl;
				st.pop_back();

				resultImage[width * ky + kx] = labelNumber;
				for (int ny = ky - 1; ny <= ky + 1; ny++) {
					// y축 범위를 벗어나는 점 제외
					if (ny < 0 || ny >= height) continue;
					for (int nx = kx - 1; nx <= kx + 1; nx++) {
						// x축 범위를 벗어나는 점 제외
						if (nx < 0 || nx >= width) continue;
						// source image가 값이 있고 labeling이 안된 좌표를 stack에 push
						if (inputImage[width * ny + nx] != 255 || resultImage[width * ny + nx] != 0) continue;
						st.push_back(labeling(nx, ny));
						test01.push_back(labeling(nx, ny));
						//std::cout << "라벨링 안 된 좌표X" << nx << std::endl;
						//std::cout << "라벨링 안 된 좌표Y" << ny << std::endl;
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

	std::cout << "최대X좌표" << MAX_x << std::endl;
	std::cout << "최대Y좌표" << MAX_y << std::endl;
	std::cout << "최소X좌표" << MIN_x << std::endl;
	std::cout << "최소Y좌표" << MIN_y << std::endl;
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
