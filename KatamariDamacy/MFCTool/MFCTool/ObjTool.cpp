// ObjTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjTool.h"
#include "afxdialogex.h"


// CObjTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CObjTool, CDialog)

CObjTool::CObjTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJTOOL, pParent)
{

}

CObjTool::~CObjTool()
{
}

void CObjTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ObjListBox);
}


BEGIN_MESSAGE_MAP(CObjTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjTool::OnLbnSelchangeObjList)
	ON_WM_DROPFILES()
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
