#define WIDTHBYTES(bits) (((bits)+31)/32*4)
// MFCApplication3Doc.h : CMFCApplication3Doc Ŭ������ �������̽�
//


#pragma once


class CMFCApplication3Doc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMFCApplication3Doc();
	DECLARE_DYNCREATE(CMFCApplication3Doc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
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

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CMFCApplication3Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void CopyClipboard(BYTE* m_CpyImg, int height, int width, int biBitCount);
	void Onskin();
	int labelMax(int a, int b);
	int labelMin(int a, int b);
};
