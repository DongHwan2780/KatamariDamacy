// ObjTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MFCToolView.h"

// CObjTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjTool, CDialog)

CObjTool::CObjTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJTOOL, pParent)
	, m_fPosX(0), m_fPosY(0), m_fPosZ(0)
	, m_fScaleX(0), m_fScaleY(0), m_fScaleZ(0)
	, m_fAngleX(0), m_fAngleY(0), m_fAngleZ(0)
{

}

CObjTool::~CObjTool()
{
}

void CObjTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ObjListBox);
	DDX_Text(pDX, IDC_EDIT2, m_fPosX);
	DDX_Text(pDX, IDC_EDIT3, m_fPosY);
	DDX_Text(pDX, IDC_EDIT4, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT5, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT6, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT7, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT8, m_fAngleX);
	DDX_Text(pDX, IDC_EDIT9, m_fAngleY);
	DDX_Text(pDX, IDC_EDIT10, m_fAngleZ);
}


BEGIN_MESSAGE_MAP(CObjTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjTool::OnLbnSelchangeObjList)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CObjTool::OnBnClickedCubeColliderCreate)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjTool::OnBnClickedSphereColliderCreate)
	ON_BN_CLICKED(IDC_BUTTON3, &CObjTool::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON4, &CObjTool::OnBnClickedSave)
END_MESSAGE_MAP()


// CObjTool 메시지 처리기입니다.


void CObjTool::OnLbnSelchangeObjList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CObjTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int iCount = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR szFilePath[MAX_PATH]{};

	for (int i = 0; i < iCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, PathFindFileName(szFilePath));
		PathRemoveExtension(szFileName);

		m_ObjListBox.AddString(szFileName);
	}

	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}


void CObjTool::OnBnClickedCubeColliderCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);


	UpdateData(FALSE);
}


void CObjTool::OnBnClickedSphereColliderCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);


	UpdateData(FALSE);
}


void CObjTool::OnBnClickedApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);


	UpdateData(FALSE);
}


void CObjTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);


	UpdateData(FALSE);
}
