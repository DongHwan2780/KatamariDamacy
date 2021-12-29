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
	DDX_Control(pDX, IDC_LIST1, m_FBXListBox);
	DDX_Text(pDX, IDC_EDIT2, m_fPosX);
	DDX_Text(pDX, IDC_EDIT3, m_fPosY);
	DDX_Text(pDX, IDC_EDIT4, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT5, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT6, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT7, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT8, m_fAngleX);
	DDX_Text(pDX, IDC_EDIT9, m_fAngleY);
	DDX_Text(pDX, IDC_EDIT10, m_fAngleZ);
	DDX_Control(pDX, IDC_LIST2, m_ObjList);
}


BEGIN_MESSAGE_MAP(CObjTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjTool::OnLbnSelchangeFBXList)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CObjTool::OnBnClickedCubeColliderCreate)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjTool::OnBnClickedSphereColliderCreate)
	ON_BN_CLICKED(IDC_BUTTON3, &CObjTool::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON4, &CObjTool::OnBnClickedSave)
	ON_LBN_SELCHANGE(IDC_LIST2, &CObjTool::OnLbnSelchangeObjList)
END_MESSAGE_MAP()


// CObjTool �޽��� ó�����Դϴ�.


void CObjTool::OnLbnSelchangeFBXList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CManagement*	m_pManagement = GET_INSTANCE(CManagement);

	m_ObjList.ResetContent();

	int iIdx = m_FBXListBox.GetCurSel();
	int ObjCnt = 0;
	CString wstrModelName;
	m_FBXListBox.GetText(iIdx, wstrModelName);
	m_strObjName = L"GameObject_";
	m_strLayerName = L"Layer_";
	m_strObjName += wstrModelName;
	m_strLayerName += wstrModelName;

	//ObjCnt = m_pManagement->GetGameObjectListSize(STATIC_SCENE, m_strLayerName.GetString());

	for (int i = 0; i < ObjCnt; ++i)
	{
		CString ObjNum;
		CString ObjName = wstrModelName;
		ObjNum.Format(L"%d", i);
		ObjName += ObjNum;

		m_ObjList.AddString(ObjName);
		RELEASE_INSTANCE(CManagement);
		UpdateData(FALSE);
	}

	RELEASE_INSTANCE(CManagement);
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

		m_FBXListBox.AddString(szFileName);
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


void CObjTool::OnLbnSelchangeObjList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CManagement*	m_pManagement = GET_INSTANCE(CManagement);

	int iIdx = m_ObjList.GetCurSel();
	_vector vPos;   /*, vAngle;*/
	_vector ScaleX, ScaleY, ScaleZ;

	if (iIdx < 0)
		return;

	m_pGameObj = m_pManagement->GetGameObject(STATIC_SCENE, m_strLayerName.GetString(), iIdx);

	m_pTransform = dynamic_cast<CTransform*>(m_pGameObj->GetComponent(/*STATIC_SCENE, m_strLayerName.GetString(), */L"Com_Transform"));

	vPos = m_pTransform->Get_State(CTransform::POSITION);
	ScaleX = m_pTransform->Get_State(CTransform::RIGHT);
	ScaleY = m_pTransform->Get_State(CTransform::UP);
	ScaleZ = m_pTransform->Get_State(CTransform::LOOK);

	m_fPosX = XMVectorGetX(vPos);

	m_fPosY = XMVectorGetY(vPos);

	m_fPosZ = XMVectorGetZ(vPos);


	m_fScaleX = XMVectorGetX(ScaleX);

	m_fScaleY = XMVectorGetY(ScaleY);

	m_fScaleZ = XMVectorGetZ(ScaleZ);

	//m_fAngleX = dynamic_cast<CTransform*>(m_pGameObj->GetComponent(L"Com_Transform"))->GetTransformDesc().vRotate.x;

	//m_fAngleY = dynamic_cast<CTransform*>(m_pGameObj->GetComponent(L"Com_Transform"))->GetTransformDesc().vRotate.y;

	//m_fAngleZ = dynamic_cast<CTransform*>(m_pGameObj->GetComponent(L"Com_Transform"))->GetTransformDesc().vRotate.z;

	UpdateData(FALSE);

	RELEASE_INSTANCE(CManagement);
}
