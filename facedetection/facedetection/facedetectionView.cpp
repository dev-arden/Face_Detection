
// facedetectionView.cpp : CfacedetectionView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CfacedetectionView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FACEDETECTION, &CfacedetectionView::OnFacedetection)
	ON_COMMAND(ID_LABELING, &CfacedetectionView::OnLabeling)
END_MESSAGE_MAP()

// CfacedetectionView 생성/소멸

CfacedetectionView::CfacedetectionView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i<256; i++)
	{
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i;
		BmInfo->bmiColors[i].rgbReserved = 0;
	}
}

CfacedetectionView::~CfacedetectionView()
{
	if (BmInfo) delete BmInfo;
}

BOOL CfacedetectionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CfacedetectionView 그리기

void CfacedetectionView::OnDraw(CDC* pDC)
{
	CfacedetectionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (pDoc->m_InImg == NULL) return;
	height = pDoc->dibHi.biHeight;
	width = pDoc->dibHi.biWidth;
	rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount*pDoc->dibHi.biWidth);
	BmInfo->bmiHeader = pDoc->dibHi;
	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height,
		0, 0, 0, height, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS);
}


// CfacedetectionView 인쇄


void CfacedetectionView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CfacedetectionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CfacedetectionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CfacedetectionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
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


// CfacedetectionView 진단

#ifdef _DEBUG
void CfacedetectionView::AssertValid() const
{
	CView::AssertValid();
}

void CfacedetectionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CfacedetectionDoc* CfacedetectionView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CfacedetectionDoc)));
	return (CfacedetectionDoc*)m_pDocument;
}
#endif //_DEBUG


// CfacedetectionView 메시지 처리기


void CfacedetectionView::OnFacedetection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CfacedetectionDoc* pDoc = GetDocument(); // Doc 클래스참조
	ASSERT_VALID(pDoc);
	pDoc->OnFacedetection(); // 함수호출
	pDoc->CopyClipboard(pDoc->m_OutImg, pDoc->dibHi.biHeight, pDoc->dibHi.biWidth, 24);
	//<== 독립된Doc.(윈도우)를열어서, Bit맵정보와데이터를출력버퍼에복사함//
	//<== 최종Bit수는입력Buffer와같게해주어야함//
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CfacedetectionView::OnLabeling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CfacedetectionDoc* pDoc = GetDocument(); // Doc 클래스참조
	ASSERT_VALID(pDoc);
	pDoc->OnLabeling(); // 함수호출
	pDoc->CopyClipboard(pDoc->m_OutImg, pDoc->dibHi.biHeight, pDoc->dibHi.biWidth, 24);
	//<== 독립된Doc.(윈도우)를열어서, Bit맵정보와데이터를출력버퍼에복사함//
	//<== 최종Bit수는입력Buffer와같게해주어야함//
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}
