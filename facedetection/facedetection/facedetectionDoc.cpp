
// facedetectionDoc.cpp : CfacedetectionDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "facedetection.h"
#endif

#include "facedetectionDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CfacedetectionDoc

IMPLEMENT_DYNCREATE(CfacedetectionDoc, CDocument)

BEGIN_MESSAGE_MAP(CfacedetectionDoc, CDocument)
END_MESSAGE_MAP()


// CfacedetectionDoc 생성/소멸

CfacedetectionDoc::CfacedetectionDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CfacedetectionDoc::~CfacedetectionDoc()
{
}

BOOL CfacedetectionDoc::OnNewDocument()
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




// CfacedetectionDoc serialization

void CfacedetectionDoc::Serialize(CArchive& ar)
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
void CfacedetectionDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CfacedetectionDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CfacedetectionDoc::SetSearchContent(const CString& value)
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

// CfacedetectionDoc 진단

#ifdef _DEBUG
void CfacedetectionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CfacedetectionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CfacedetectionDoc 명령


BOOL CfacedetectionDoc::OnOpenDocument(LPCTSTR lpszPathName)
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


	return TRUE;
}


//void CfacedetectionDoc::CopyClipboard()
//{
//}


void CfacedetectionDoc::CopyClipboard(BYTE* m_CpyImg, int height, int width, int biBitCount)
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


void CfacedetectionDoc::OnFacedetection()
{
	/*
	Y = 0.257*R + 0.504*G + 0.098*B + 16;
	Cb = -0.148*R - 0.291*G + 0.439*B + 128;
	Cr = 0.439*R - 0.368*G - 0.071*B + 128;
	*/
	//CDialog22 dlg;
	int i, num_C, R, G, B, size;
	int Y, Cb, Cr;
	int mean;
	if (dibHi.biBitCount == 24)
		num_C = 3;
	else if (dibHi.biBitCount == 8)
		num_C = 1;
	height = dibHi.biHeight;
	width = dibHi.biWidth;
	size = height*width;

	//dlg.DoModal();

	if (dibHi.biBitCount == 24)
	{
		m_OutImg = new BYTE[size * 3];
		for (i = 0; i < size; i++)
		{
			B = (int)m_InImg[i*num_C];
			G = (int)m_InImg[i*num_C + 1];
			R = (int)m_InImg[i*num_C + 2];//mean = (pixB + pixG + pixR) / 3;
			Y = 0.257*R + 0.504*G + 0.098*B + 16;
			Cb = -0.148*R - 0.291*G + 0.439*B + 128;
			Cr = 0.439*R - 0.368*G - 0.071*B + 128;
			
			if (77 < Cb && Cb < 127 && 133 < Cr && Cr < 173)
			{
				m_OutImg[i*num_C] = 255;
				m_OutImg[i*num_C + 1] = 255;
				m_OutImg[i*num_C + 2] = 255;
			}		
			else
			{
				m_OutImg[i*num_C] = 0;
				m_OutImg[i*num_C + 1] = 0;
				m_OutImg[i*num_C + 2] = 0;
			}
		}
	}
}
