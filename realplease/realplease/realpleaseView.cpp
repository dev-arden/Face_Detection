
// realpleaseView.cpp : CrealpleaseView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CrealpleaseView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SKINDETECTION, &CrealpleaseView::OnSkindetection)
END_MESSAGE_MAP()

// CrealpleaseView 생성/소멸

CrealpleaseView::CrealpleaseView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CrealpleaseView 그리기

void CrealpleaseView::OnDraw(CDC* pDC)
{
	CrealpleaseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// OnDraw2) : 화면에출력을담담하는함수
	if (pDoc->m_InImg == NULL) return;
	height = pDoc->dibHi.biHeight;
	width = pDoc->dibHi.biWidth;
	rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount*pDoc->dibHi.biWidth);
	BmInfo->bmiHeader = pDoc->dibHi;
	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height,
		0, 0, 0, height, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS);
}


// CrealpleaseView 인쇄


void CrealpleaseView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CrealpleaseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CrealpleaseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CrealpleaseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
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


// CrealpleaseView 진단

#ifdef _DEBUG
void CrealpleaseView::AssertValid() const
{
	CView::AssertValid();
}

void CrealpleaseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CrealpleaseDoc* CrealpleaseView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CrealpleaseDoc)));
	return (CrealpleaseDoc*)m_pDocument;
}
#endif //_DEBUG


// CrealpleaseView 메시지 처리기


void CrealpleaseView::OnSkindetection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CrealpleaseDoc* pDoc = GetDocument(); // Doc 클래스참조
	ASSERT_VALID(pDoc);
	pDoc->OnSKindetection(); // 함수호출
	pDoc->CopyClipboard(pDoc->m_OutImg, pDoc->dibHi.biHeight, pDoc->dibHi.biWidth, 24);
	//<== 독립된Doc.(윈도우)를열어서, Bit맵정보와데이터를출력버퍼에복사함//
	//<== 최종Bit수는입력Buffer와같게해주어야함//
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}
