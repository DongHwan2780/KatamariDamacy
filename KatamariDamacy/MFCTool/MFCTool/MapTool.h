#pragma once


// CMapTool 대화 상자입니다.
#include "Management.h"
#include "ClientDefines.h"

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonAll();
	afx_msg void OnBnClickedButtonApplyWidth();
	afx_msg void OnBnClickedButtonApplyLength();
	int m_iTileX;
	int m_iTileY;
	float m_fTileInterval;
	int m_iIndex;
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	int m_iWidthLength;

	_uint iIndex = 0;

	CVIBuffer_Terrain* m_pVIBuffer = nullptr;

	void Update_Dialog() { UpdateData(FALSE); }
	afx_msg void OnBnClickedLoad();
};
