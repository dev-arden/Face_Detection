#define WIDTHBYTES(bits) (((bits)+31)/32*4)
// MFCApplication3Doc.h : CMFCApplication3Doc 클래스의 인터페이스
//


#pragma once


class CMFCApplication3Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CMFCApplication3Doc();
	DECLARE_DYNCREATE(CMFCApplication3Doc)

// 특성입니다.
public:

// 작업입니다.
public:
	int m_Re_size;
	int m_Re_height;
	int m_Re_width;
	unsigned char *m_OutputImage;
	int m_width;
	int m_height;
	int m_size;
	unsigned char *m_InputImage;

	int ImgType;
	int height, width;
	RGBQUAD palRGB[256];
	BITMAPINFOHEADER dibHi;
	BITMAPFILEHEADER dibHf;
	BYTE *m_InImg, *m_OutImg, *m_COrigin, *m_Test;

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CMFCApplication3Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void CopyClipboard(BYTE* m_CpyImg, int height, int width, int biBitCount);
	void Onskin();
	int labelMax(int a, int b);
	int labelMin(int a, int b);
};
