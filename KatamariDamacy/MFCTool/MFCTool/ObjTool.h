#pragma once
#include "afxwin.h"


// CObjTool ��ȭ �����Դϴ�.

class CObjTool : public CDialog
{
	DECLARE_DYNAMIC(CObjTool)

public:
	CObjTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CObjTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeObjList();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CListBox m_ObjListBox;
};
