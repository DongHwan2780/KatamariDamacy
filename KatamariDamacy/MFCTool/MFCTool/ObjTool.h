#pragma once
#include "afxwin.h"


// CObjTool 대화 상자입니다.

class CObjTool : public CDialog
{
	DECLARE_DYNAMIC(CObjTool)

public:
	CObjTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeObjList();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CListBox m_ObjListBox;
	afx_msg void OnBnClickedCubeColliderCreate();
	afx_msg void OnBnClickedSphereColliderCreate();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedSave();
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	float m_fAngleX;
	float m_fAngleY;
	float m_fAngleZ;
};
