// Form.cpp : ���� �����Դϴ�.
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


// CForm �����Դϴ�.

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


// CForm �޽��� ó�����Դϴ�.


void CForm::OnTcnSelchangeTab_Main(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
