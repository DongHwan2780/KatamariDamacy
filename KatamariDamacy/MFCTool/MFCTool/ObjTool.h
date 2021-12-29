#pragma once
#include "afxwin.h"

#include "Management.h"
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
	afx_msg void OnLbnSelchangeFBXList();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CListBox m_FBXListBox;
	CListBox m_ObjList;
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
	CString	m_strLayerName = L"Layer_";
	CString	m_strObjName = L"GameObject_";
	afx_msg void OnLbnSelchangeObjList();

	CObj* m_pGameObj;
	CTransform*  m_pTransform;
};
