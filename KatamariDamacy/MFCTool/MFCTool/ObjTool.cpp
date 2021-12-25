// ObjTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MFCToolView.h"

// CObjTool ��ȭ �����Դϴ�.

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


// CObjTool �޽��� ó�����Դϴ�.


void CObjTool::OnLbnSelchangeObjList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CObjTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);


	UpdateData(FALSE);
}


void CObjTool::OnBnClickedSphereColliderCreate()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);


	UpdateData(FALSE);
}


void CObjTool::OnBnClickedApply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);


	UpdateData(FALSE);
}


void CObjTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);


	UpdateData(FALSE);
}
