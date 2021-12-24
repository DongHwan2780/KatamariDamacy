// ObjTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjTool.h"
#include "afxdialogex.h"


// CObjTool 대화 상자입니다.

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
