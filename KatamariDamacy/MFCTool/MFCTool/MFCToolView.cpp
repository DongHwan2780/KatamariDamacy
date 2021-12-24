
// MFCToolView.cpp : CMFCToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCTool.h"
#endif

#include "MFCToolDoc.h"
#include "MFCToolView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
HWND g_hWnd;

// CMFCToolView]

IMPLEMENT_DYNCREATE(CMFCToolView, CView)

BEGIN_MESSAGE_MAP(CMFCToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFCToolView 생성/소멸

CMFCToolView::CMFCToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCToolView::~CMFCToolView()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);
	CManagement::Release_Engine();
}

BOOL CMFCToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCToolView 그리기

void CMFCToolView::OnDraw(CDC* /*pDC*/)
{
	CMFCToolDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pManagement->UpdateTool();

	Invalidate(false);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFCToolView 인쇄

BOOL CMFCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCToolView 진단

#ifdef _DEBUG
void CMFCToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}


CMFCToolDoc* CMFCToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolDoc)));
	return (CMFCToolDoc*)m_pDocument;
}
#endif //_DEBUG

// CMFCToolView 메시지 처리기


void CMFCToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;
	m_pManagement = CManagement::GetInstance();
	Safe_AddRef(m_pManagement);

	//CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	//RECT rcMain{};

	//pMain->GetWindowRect(&rcMain);
	//rcMain.right = rcMain.right - rcMain.left;
	//rcMain.bottom = rcMain.bottom - rcMain.top;
	//rcMain.left = 0;
	//rcMain.top = 0;

	//RECT rcView{};
	//GetClientRect(&rcView);
	//int iGapX = rcMain.right - rcView.right;
	//int iGapY = rcMain.bottom - rcView.bottom;

	//pMain->SetWindowPos(nullptr, 0, 0, 800 + iGapX + 1, 600 + iGapY + 1, SWP_NOMOVE);

	HRESULT hr;
	hr = m_pManagement->Ready_GraphicDevice(g_hWnd, 800, 600, &m_pDevice, &m_pDeviceContext);

	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}
