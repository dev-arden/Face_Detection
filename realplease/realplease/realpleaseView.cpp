
// realpleaseView.cpp : CrealpleaseView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "realplease.h"
#endif

#include "realpleaseDoc.h"
#include "realpleaseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CrealpleaseView

IMPLEMENT_DYNCREATE(CrealpleaseView, CView)

BEGIN_MESSAGE_MAP(CrealpleaseView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CrealpleaseView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SKINDETECTION, &CrealpleaseView::OnSkindetection)
END_MESSAGE_MAP()

// CrealpleaseView ����/�Ҹ�

CrealpleaseView::CrealpleaseView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i<256; i++)
	{
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i;
		BmInfo->bmiColors[i].rgbReserved = 0;
	}

}

CrealpleaseView::~CrealpleaseView()
{
	if (BmInfo) delete BmInfo;
}

BOOL CrealpleaseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CrealpleaseView �׸���

void CrealpleaseView::OnDraw(CDC* pDC)
{
	CrealpleaseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	// OnDraw2) : ȭ�鿡���������ϴ��Լ�
	if (pDoc->m_InImg == NULL) return;
	height = pDoc->dibHi.biHeight;
	width = pDoc->dibHi.biWidth;
	rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount*pDoc->dibHi.biWidth);
	BmInfo->bmiHeader = pDoc->dibHi;
	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height,
		0, 0, 0, height, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS);
}


// CrealpleaseView �μ�


void CrealpleaseView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CrealpleaseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CrealpleaseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CrealpleaseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CrealpleaseView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CrealpleaseView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CrealpleaseView ����

#ifdef _DEBUG
void CrealpleaseView::AssertValid() const
{
	CView::AssertValid();
}

void CrealpleaseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CrealpleaseDoc* CrealpleaseView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CrealpleaseDoc)));
	return (CrealpleaseDoc*)m_pDocument;
}
#endif //_DEBUG


// CrealpleaseView �޽��� ó����


void CrealpleaseView::OnSkindetection()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CrealpleaseDoc* pDoc = GetDocument(); // Doc Ŭ��������
	ASSERT_VALID(pDoc);
	pDoc->OnSKindetection(); // �Լ�ȣ��
	pDoc->CopyClipboard(pDoc->m_OutImg, pDoc->dibHi.biHeight, pDoc->dibHi.biWidth, 24);
	//<== ������Doc.(������)�����, Bit�������͵����͸���¹��ۿ�������//
	//<== ����Bit�����Է�Buffer�Ͱ������־����//
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}
