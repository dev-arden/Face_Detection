#define WIDTHBYTES(bits) (((bits)+31)/32*4)
// realpleaseDoc.h : CrealpleaseDoc Ŭ������ �������̽�
//


#pragma once


class CrealpleaseDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CrealpleaseDoc();
	DECLARE_DYNCREATE(CrealpleaseDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
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
	virtual ~CrealpleaseDoc();
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
	void OnSKindetection();
	//int  DibLabeling(CDib& dib);
};
