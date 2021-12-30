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
	ON_EN_CHANGE(IDC_EDIT2, &CObjTool::OnEnChangePosX)
	ON_EN_CHANGE(IDC_EDIT3, &CObjTool::OnEnChangePosY)
	ON_EN_CHANGE(IDC_EDIT4, &CObjTool::OnEnChangePosZ)
	ON_EN_CHANGE(IDC_EDIT5, &CObjTool::OnEnChangeScaleX)
	ON_EN_CHANGE(IDC_EDIT6, &CObjTool::OnEnChangeScaleY)
	ON_EN_CHANGE(IDC_EDIT7, &CObjTool::OnEnChangeScaleZ)
	ON_EN_CHANGE(IDC_EDIT8, &CObjTool::OnEnChangeAngleX)
	ON_EN_CHANGE(IDC_EDIT9, &CObjTool::OnEnChangeAngleY)
	ON_EN_CHANGE(IDC_EDIT10, &CObjTool::OnEnChangeAngleZ)
END_MESSAGE_MAP()


// CObjTool �޽��� ó�����Դϴ�.


void CObjTool::OnLbnSelchangeFBXList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CManagement*	m_pManagement = GET_INSTANCE(CManagement);

	//m_ObjList.ResetContent();

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
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCToolView* pView = dynamic_cast<CMFCToolView*>(pMain->m_tMainSplitter.GetPane(0, 1));

	CManagement*	m_pManagement = GET_INSTANCE(CManagement);


	CFileDialog Dlg(FALSE,// ���� ���(TRUE) Ȥ�� ���� ���(FALSE) ��� �� ������. 
		L"dat",// ����Ʈ Ȯ���� �� 
		L"*.dat",// ����Ʈ ���� �̸� 
		OFN_OVERWRITEPROMPT);// â�� �⺻ ���¸� �������� �� ����. �ִ� �ߺ����� ����� ���޽��� �����.  
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Data\\Object");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		_uint MaxCnt = m_ObjList.GetCount();
		_uint iModelIdx = m_FBXListBox.GetCurSel();
		CString wstrModelName;
		CString wstrLayerName = L"Layer_";

		CObj* pGameObj = nullptr;
		_float3 vPosition = { 0.f, 0.f, 0.f };
		_float3 vScale = { 0.f,0.f,0.f };
		_float3 vRotate = { 0.f, 0.f, 0.f };

		m_FBXListBox.GetText(iModelIdx, wstrModelName);
		wstrLayerName += wstrModelName;

		for (_uint i = 0; i < MaxCnt; ++i)
		{

			//pGameObj = m_pManagement->GetGameObject(STATIC_SCENE, wstrLayerName.GetString(), i);
			//m_pTransform = dynamic_cast<CTransform*>(m_pGameObj->GetComponent(L"Com_Transform"));

			//XMStoreFloat3(&vPosition, m_pTransform->Get_State(CTransform::POSITION));

			//vScale = dynamic_cast<CTransform*>(pGameObj->GetComponent(L"Com_Transform"))->GetTransformDesc().vScale;
			//vRotate = dynamic_cast<CTransform*>(pGameObj->GetComponent(L"Com_Transform"))->GetTransformDesc().vRotate;

			//WriteFile(hFile, vPosition, sizeof(_float3), &dwByte, nullptr);
			//WriteFile(hFile, vScale, sizeof(_float3), &dwByte, nullptr);
			//WriteFile(hFile, vRotate, sizeof(_float3), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}

	RELEASE_INSTANCE(CManagement);
}


void CObjTool::OnLbnSelchangeObjList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CManagement*	m_pManagement = GET_INSTANCE(CManagement);

	int iIdx = m_ObjList.GetCurSel();
	_vector vPos;   /*, vAngle;*/

	if (iIdx < 0)
		return;

	m_pGameObj = m_pManagement->GetGameObject(STATIC_SCENE, m_strLayerName.GetString(), iIdx);

	m_pTransform = dynamic_cast<CTransform*>(m_pGameObj->GetComponent(L"Com_Transform"));

	vPos = m_pTransform->Get_State(CTransform::POSITION);

	// ������
	m_fPosX = XMVectorGetX(vPos);
	m_fPosY = XMVectorGetY(vPos);
	m_fPosZ = XMVectorGetZ(vPos);

	// ������
	//m_fScaleX = m_pTransform->Get_Scale(CTransform::RIGHT);
	//m_fScaleY = m_pTransform->Get_Scale(CTransform::UP);
	//m_fScaleZ = m_pTransform->Get_Scale(CTransform::LOOK);

	//m_fAngleX = dynamic_cast<CTransform*>(m_pGameObj->GetComponent(L"Com_Transform"))->GetTransformDesc().vRotate.x;

	//m_fAngleY = dynamic_cast<CTransform*>(m_pGameObj->GetComponent(L"Com_Transform"))->GetTransformDesc().vRotate.y;

	//m_fAngleZ = dynamic_cast<CTransform*>(m_pGameObj->GetComponent(L"Com_Transform"))->GetTransformDesc().vRotate.z;

	RELEASE_INSTANCE(CManagement);
	UpdateData(FALSE);

}


void CObjTool::OnEnChangePosX()
{
	UpdateData(TRUE);
	m_pTransform->Set_PosX(CTransform::POSITION, m_fPosX);
	UpdateData(FALSE);
}


void CObjTool::OnEnChangePosY()
{
	UpdateData(TRUE);
	m_pTransform->Set_PosY(CTransform::POSITION, m_fPosY);
	UpdateData(FALSE);
}


void CObjTool::OnEnChangePosZ()
{
	UpdateData(TRUE);
	m_pTransform->Set_PosZ(CTransform::POSITION, m_fPosZ);
	UpdateData(FALSE);
}


void CObjTool::OnEnChangeScaleX()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}


void CObjTool::OnEnChangeScaleY()
{
	UpdateData(TRUE);
	
	UpdateData(FALSE);
}


void CObjTool::OnEnChangeScaleZ()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}


void CObjTool::OnEnChangeAngleX()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}


void CObjTool::OnEnChangeAngleY()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}


void CObjTool::OnEnChangeAngleZ()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}
