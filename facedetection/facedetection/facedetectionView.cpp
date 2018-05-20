
// facedetectionView.cpp : CfacedetectionView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "facedetection.h"
#endif

#include "facedetectionDoc.h"
#include "facedetectionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfacedetectionView

IMPLEMENT_DYNCREATE(CfacedetectionView, CView)

BEGIN_MESSAGE_MAP(CfacedetectionView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CfacedetectionView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CfacedetectionView ����/�Ҹ�

CfacedetectionView::CfacedetectionView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CfacedetectionView::~CfacedetectionView()
{
}

BOOL CfacedetectionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CfacedetectionView �׸���

void CfacedetectionView::OnDraw(CDC* /*pDC*/)
{
	CfacedetectionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CfacedetectionView �μ�


void CfacedetectionView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CfacedetectionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CfacedetectionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CfacedetectionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CfacedetectionView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CfacedetectionView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CfacedetectionView ����

#ifdef _DEBUG
void CfacedetectionView::AssertValid() const
{
	CView::AssertValid();
}

void CfacedetectionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CfacedetectionDoc* CfacedetectionView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CfacedetectionDoc)));
	return (CfacedetectionDoc*)m_pDocument;
}
#endif //_DEBUG


// CfacedetectionView �޽��� ó����
