#pragma once

#include "ObjTool.h"
#include "afxcmn.h"

// CForm 폼 뷰입니다.

class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CObjTool	m_tObjTool;
	CTabCtrl m_CtrlTab_Main;
	afx_msg void OnTcnSelchangeTab_Main(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
};


