
// MFCApplication_skinView.h : CMFCApplication_skinView Ŭ������ �������̽�
//

#pragma once


class CMFCApplication_skinView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFCApplication_skinView();
	DECLARE_DYNCREATE(CMFCApplication_skinView)

// Ư���Դϴ�.
public:
	CMFCApplication_skinDoc* GetDocument() const;

// �۾��Դϴ�.
public:

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
	virtual ~CMFCApplication_skinView();
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
};

#ifndef _DEBUG  // MFCApplication_skinView.cpp�� ����� ����
inline CMFCApplication_skinDoc* CMFCApplication_skinView::GetDocument() const
   { return reinterpret_cast<CMFCApplication_skinDoc*>(m_pDocument); }
#endif

