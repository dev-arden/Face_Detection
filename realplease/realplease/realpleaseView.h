
// realpleaseView.h : CrealpleaseView Ŭ������ �������̽�
//

#pragma once


class CrealpleaseView : public CView
{
protected: // serialization������ ��������ϴ�.
	CrealpleaseView();
	DECLARE_DYNCREATE(CrealpleaseView)

// Ư���Դϴ�.
public:
	CrealpleaseDoc* GetDocument() const;

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
	virtual ~CrealpleaseView();
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
	afx_msg void OnSkindetection();
};

#ifndef _DEBUG  // realpleaseView.cpp�� ����� ����
inline CrealpleaseDoc* CrealpleaseView::GetDocument() const
   { return reinterpret_cast<CrealpleaseDoc*>(m_pDocument); }
#endif

