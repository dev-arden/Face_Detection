#define WIDTHBYTES(bits) (((bits)+31)/32*4)
// realpleaseDoc.h : CrealpleaseDoc 클래스의 인터페이스
//


#pragma once


class CrealpleaseDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CrealpleaseDoc();
	DECLARE_DYNCREATE(CrealpleaseDoc)

// 특성입니다.
public:

// 작업입니다.
public:
	int ImgType;
	int height, width;
	RGBQUAD palRGB[256];
	BITMAPINFOHEADER dibHi;
	BITMAPFILEHEADER dibHf;

	BYTE *m_InImg, *m_OutImg;
	BYTE *m_GrayImg, *m_GrayImg2;
	BYTE *m_R, *m_G, *m_B;
	BYTE *m_CpyImg;
	int *m_mRGB;

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
	virtual ~CrealpleaseDoc();
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
	void OnSKindetection();
	//int  DibLabeling(CDib& dib);
};
