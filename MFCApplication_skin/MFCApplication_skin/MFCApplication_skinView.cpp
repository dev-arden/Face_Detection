
// MFCApplication_skinView.cpp : CMFCApplication_skinView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFCApplication_skin.h"
#endif

#include "MFCApplication_skinDoc.h"
#include "MFCApplication_skinView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication_skinView

IMPLEMENT_DYNCREATE(CMFCApplication_skinView, CView)

BEGIN_MESSAGE_MAP(CMFCApplication_skinView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication_skinView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFCApplication_skinView ����/�Ҹ�

CMFCApplication_skinView::CMFCApplication_skinView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFCApplication_skinView::~CMFCApplication_skinView()
{
}

BOOL CMFCApplication_skinView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFCApplication_skinView �׸���

void CMFCApplication_skinView::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication_skinDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMFCApplication_skinView �μ�


void CMFCApplication_skinView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication_skinView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFCApplication_skinView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFCApplication_skinView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CMFCApplication_skinView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication_skinView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication_skinView ����

#ifdef _DEBUG
void CMFCApplication_skinView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication_skinView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication_skinDoc* CMFCApplication_skinView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication_skinDoc)));
	return (CMFCApplication_skinDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication_skinView �޽��� ó����
