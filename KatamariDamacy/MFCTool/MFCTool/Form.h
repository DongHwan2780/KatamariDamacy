#pragma once

#include "ObjTool.h"
#include "afxcmn.h"

// CForm �� ���Դϴ�.

class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void Initialize();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CObjTool	m_tObjTool;
	CTabCtrl m_CtrlTab_Main;
	afx_msg void OnTcnSelchangeTab_Main(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
};


