// Form.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "Form.h"
// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
{

}

CForm::~CForm()
{
}

void CForm::Initialize()
{
	CRect rc;
	m_CtrlTab_Main.GetWindowRect(rc);

	m_CtrlTab_Main.InsertItem(0, L"ObjTool");
	m_CtrlTab_Main.SetCurSel(0);

	m_tObjTool.Create(IDD_OBJTOOL, &m_CtrlTab_Main);
	m_tObjTool.MoveWindow(0, 20, rc.Width() - 5, rc.Height() - 5);
	m_tObjTool.ShowWindow(SW_SHOW);
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_CtrlTab_Main);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CForm::OnTcnSelchangeTab_Main)
END_MESSAGE_MAP()


// CForm 진단입니다.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기입니다.


void CForm::OnTcnSelchangeTab_Main(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//int nIndex = m_CtrlTab_Main.GetCurSel();
	//switch (nIndex)
	//{
	//case 0:
	//	m_tObjTool.ShowWindow(SW_SHOW);
	//	break;
	//}
	//*pResult = 0;
}

void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}
