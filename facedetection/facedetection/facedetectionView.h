
// facedetectionView.h : CfacedetectionView Ŭ������ �������̽�
//

#pragma once


class CfacedetectionView : public CView
{
protected: // serialization������ ��������ϴ�.
	CfacedetectionView();
	DECLARE_DYNCREATE(CfacedetectionView)

// Ư���Դϴ�.
public:
	CfacedetectionDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	int rwsize;
	int height;
	int width;
	BITMAPINFO *BmInfo;

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CfacedetectionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFacedetection();
};

#ifndef _DEBUG  // facedetectionView.cpp�� ����� ����
inline CfacedetectionDoc* CfacedetectionView::GetDocument() const
   { return reinterpret_cast<CfacedetectionDoc*>(m_pDocument); }
#endif

