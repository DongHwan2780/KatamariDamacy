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
	, m_fScaleX(0)
	, m_fColPosX(0), m_fColPosY(0), m_fColPosZ(0)
	, m_fColScaleX(0), m_fColScaleY(0), m_fColScaleZ(0)
{

}

CObjTool::~CObjTool()
{
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
}

void CObjTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_FBXListBox);
	DDX_Text(pDX, IDC_EDIT2, m_fPosX);
	DDX_Text(pDX, IDC_EDIT3, m_fPosY);
	DDX_Text(pDX, IDC_EDIT4, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT5, m_fScaleX);
	DDX_Control(pDX, IDC_LIST2, m_ObjList);
	DDX_Text(pDX, IDC_EDIT14, m_fColPosX);
	DDX_Text(pDX, IDC_EDIT15, m_fColPosY);
	DDX_Text(pDX, IDC_EDIT16, m_fColPosZ);
	DDX_Text(pDX, IDC_EDIT11, m_fColScaleX);
	DDX_Text(pDX, IDC_EDIT12, m_fColScaleY);
	DDX_Text(pDX, IDC_EDIT13, m_fColScaleZ);
	DDX_Control(pDX, IDC_SLIDER1, m_fScrollPosX);
	DDX_Control(pDX, IDC_SLIDER2, m_fScrollPosY);
	DDX_Control(pDX, IDC_SLIDER3, m_fScrollPosZ);
}


BEGIN_MESSAGE_MAP(CObjTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjTool::OnLbnSelchangeFBXList)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CObjTool::OnBnClickedCubeColliderCreate)

	ON_BN_CLICKED(IDC_BUTTON3, &CObjTool::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON4, &CObjTool::OnBnClickedSave)
	ON_LBN_SELCHANGE(IDC_LIST2, &CObjTool::OnLbnSelchangeObjList)
	ON_EN_CHANGE(IDC_EDIT2, &CObjTool::OnEnChangePosX)
	ON_EN_CHANGE(IDC_EDIT3, &CObjTool::OnEnChangePosY)
	ON_EN_CHANGE(IDC_EDIT4, &CObjTool::OnEnChangePosZ)
	ON_EN_CHANGE(IDC_EDIT5, &CObjTool::OnEnChangeScaleX)

	ON_EN_CHANGE(IDC_EDIT14, &CObjTool::OnEnChangeCollPosX)
	ON_EN_CHANGE(IDC_EDIT15, &CObjTool::OnEnChangeCollPosY)
	ON_EN_CHANGE(IDC_EDIT16, &CObjTool::OnEnChangeCollPosZ)
	ON_EN_CHANGE(IDC_EDIT11, &CObjTool::OnEnChangeCollScaleX)
	ON_EN_CHANGE(IDC_EDIT12, &CObjTool::OnEnChangeCollScaleY)
	ON_EN_CHANGE(IDC_EDIT13, &CObjTool::OnEnChangeCollScaleZ)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON2, &CObjTool::OnBnClickedColliderSave)
END_MESSAGE_MAP()


// CObjTool 메시지 처리기입니다.


void CObjTool::OnLbnSelchangeFBXList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	ObjCnt = m_pManagement->GetGameObjectListSize(STATIC_SCENE, m_strLayerName.GetString());

	for (int i = 0; i < ObjCnt; ++i)
	{
		CString ObjNum;
		CString ObjName = wstrModelName;
		ObjNum.Format(L"%d", i);
		ObjName += ObjNum;

		m_ObjList.AddString(ObjName);
	}

	UpdateData(FALSE);
	RELEASE_INSTANCE(CManagement);
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

		m_FBXListBox.AddString(szFileName);
	}

	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}


void CObjTool::OnBnClickedCubeColliderCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_FBXListBox.AddString(L"Dummy");

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
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCToolView* pView = dynamic_cast<CMFCToolView*>(pMain->m_tMainSplitter.GetPane(0, 1));
	pView->m_bInvalidate = false;

	CManagement*	m_pManagement = GET_INSTANCE(CManagement);


	CFileDialog Dlg(FALSE,// 열기 모드(TRUE) 혹은 저장 모드(FALSE) 어떤것 할 것인지. 
		L"dat",// 디폴트 확장자 명 
		L"*.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 창에 기본 상태를 설정해줄 수 있음. 애는 중복파일 저장시 경고메시지 띄워줌.  
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
		CTransform* pTransform = nullptr;

		_vector vPosition;
		_float vScale = 0.f;
		_vector vRotate = { 0.f, 0.f, 0.f };

		m_FBXListBox.GetText(iModelIdx, wstrModelName);
		wstrLayerName += wstrModelName;

		for (_uint i = 0; i < MaxCnt; ++i)
		{
			pGameObj = m_pManagement->GetGameObject(STATIC_SCENE, wstrLayerName.GetString(), i);
			pTransform = dynamic_cast<CTransform*>(pGameObj->GetComponent(L"Com_Transform"));

			vPosition = pTransform->Get_State(CTransform::POSITION);
			vScale = pTransform->Get_Scale(CTransform::RIGHT);

			WriteFile(hFile, &vPosition, sizeof(_vector), &dwByte, nullptr);
			WriteFile(hFile, &vScale, sizeof(_float), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
	pView->m_bInvalidate = true;
	RELEASE_INSTANCE(CManagement);
}



void CObjTool::OnLbnSelchangeObjList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CManagement*	m_pManagement = GET_INSTANCE(CManagement);

	int iIdx = m_ObjList.GetCurSel();
	_vector vPos;   /*, vAngle;*/

	if (iIdx < 0)
		return;

	m_pGameObj = m_pManagement->GetGameObject(STATIC_SCENE, m_strLayerName.GetString(), iIdx);

	m_pTransform = dynamic_cast<CTransform*>(m_pGameObj->GetComponent(L"Com_Transform"));

	if (!lstrcmp(m_strLayerName.GetString(), L"Layer_Dummy"))
	{
		m_pCollider = dynamic_cast<CCollider*>(m_pGameObj->GetComponent(L"Com_OBB"));
		vPos = m_pTransform->Get_State(CTransform::POSITION);

		m_fColPosX = XMVectorGetX(vPos);
		m_fColPosY = XMVectorGetY(vPos);
		m_fColPosZ = XMVectorGetZ(vPos);

		m_fColScaleX = m_pCollider->GetTransformDesc().vSize.x;
		m_fColScaleY = m_pCollider->GetTransformDesc().vSize.y;
		m_fColScaleZ = m_pCollider->GetTransformDesc().vSize.z;

		m_fScrollPosX.SetPos(m_fColPosX * 10);
		m_fScrollPosY.SetPos(m_fColPosY * 10);
		m_fScrollPosZ.SetPos(m_fColPosZ * 10);
	}
	else
	{
		vPos = m_pTransform->Get_State(CTransform::POSITION);

		// 포지션
		m_fPosX = XMVectorGetX(vPos);
		m_fPosY = XMVectorGetY(vPos);
		m_fPosZ = XMVectorGetZ(vPos);

		// 스케일
		m_fScaleX = m_pTransform->Get_Scale(CTransform::RIGHT);
	}

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
	_vector vScale = { m_fScaleX, m_fScaleX, m_fScaleX, 1.f };
	m_pTransform->Set_Scale(vScale);
	UpdateData(FALSE);
}


void CObjTool::OnEnChangeCollPosX()
{
	UpdateData(TRUE);

	m_fScrollPosX.SetPos(m_fColPosX * 10);

	m_pTransform->Set_PosX(CTransform::POSITION, m_fColPosX);
	UpdateData(FALSE);
}


void CObjTool::OnEnChangeCollPosY()
{
	UpdateData(TRUE);
	m_fScrollPosY.SetPos(m_fColPosY * 10);

	m_pTransform->Set_PosY(CTransform::POSITION, m_fColPosY);
	UpdateData(FALSE);
}


void CObjTool::OnEnChangeCollPosZ()
{
	UpdateData(TRUE);
	m_fScrollPosZ.SetPos(m_fColPosZ * 10);

	m_pTransform->Set_PosZ(CTransform::POSITION, m_fColPosZ);
	UpdateData(FALSE);
}


void CObjTool::OnEnChangeCollScaleX()
{
	UpdateData(TRUE);
	_float3 vScale = _float3(m_fColScaleX, m_fColScaleY, m_fColScaleZ);
	m_pCollider->Set_Points(vScale);

	//m_pCollider->Set_Scale(XMVectorSet(m_fColScaleX, m_fColScaleY, m_fColScaleZ, 1.f));

	UpdateData(FALSE);
}


void CObjTool::OnEnChangeCollScaleY()
{
	UpdateData(TRUE);
	_float3 vScale = _float3(m_fColScaleX, m_fColScaleY, m_fColScaleZ);
	m_pCollider->Set_Points(vScale);
	//m_pCollider->Set_Scale(XMVectorSet(m_fColScaleX, m_fColScaleY, m_fColScaleZ, 1.f));
	UpdateData(FALSE);
}


void CObjTool::OnEnChangeCollScaleZ()
{
	UpdateData(TRUE);
	_float3 vScale = _float3(m_fColScaleX, m_fColScaleY, m_fColScaleZ);
	m_pCollider->Set_Points(vScale);
	//m_pCollider->Set_Scale(XMVectorSet(m_fColScaleX, m_fColScaleY, m_fColScaleZ, 1.f));
	UpdateData(FALSE);
}


void CObjTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

	m_fColPosX = (float)m_fScrollPosX.GetPos() / 10.f;
	m_fColPosY = (float)m_fScrollPosY.GetPos() / 10.f;
	m_fColPosZ = (float)m_fScrollPosZ.GetPos() / 10.f;


	m_pTransform->Set_PosX(CTransform::POSITION, m_fColPosX);
	m_pTransform->Set_PosY(CTransform::POSITION, m_fColPosY);
	m_pTransform->Set_PosZ(CTransform::POSITION, m_fColPosZ);


	UpdateData(FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CObjTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);


	m_fScrollPosX.SetPos(m_fColPosX * 10);
	m_fScrollPosY.SetPos(m_fColPosY * 10);
	m_fScrollPosZ.SetPos(m_fColPosZ * 10);


	m_fScrollPosX.SetTicFreq(1);
	m_fScrollPosY.SetTicFreq(1);
	m_fScrollPosZ.SetTicFreq(1);

	m_fScrollPosX.SetRange(-100, 4500);
	m_fScrollPosY.SetRange(-500, 500);
	m_fScrollPosZ.SetRange(-100, 4500);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjTool::OnBnClickedColliderSave()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCToolView* pView = dynamic_cast<CMFCToolView*>(pMain->m_tMainSplitter.GetPane(0, 1));
	pView->m_bInvalidate = false;

	CManagement*	m_pManagement = GET_INSTANCE(CManagement);


	CFileDialog Dlg(FALSE,// 열기 모드(TRUE) 혹은 저장 모드(FALSE) 어떤것 할 것인지. 
		L"dat",// 디폴트 확장자 명 
		L"*.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 창에 기본 상태를 설정해줄 수 있음. 애는 중복파일 저장시 경고메시지 띄워줌.  
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
		CTransform* pTransform = nullptr;
		CCollider* pCollider = nullptr;

		_vector vPosition;
		_float3 vScale;

		m_FBXListBox.GetText(iModelIdx, wstrModelName);
		wstrLayerName += wstrModelName;

		for (_uint i = 0; i < MaxCnt; ++i)
		{
			pGameObj = m_pManagement->GetGameObject(STATIC_SCENE, wstrLayerName.GetString(), i);
			pTransform = dynamic_cast<CTransform*>(pGameObj->GetComponent(L"Com_Transform"));
			pCollider = dynamic_cast<CCollider*>(pGameObj->GetComponent(L"Com_OBB"));

			vPosition = pTransform->Get_State(CTransform::POSITION);
			vScale = pCollider->GetTransformDesc().vSize;

			WriteFile(hFile, &vPosition, sizeof(_vector), &dwByte, nullptr);
			WriteFile(hFile, &vScale, sizeof(_float3), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
	pView->m_bInvalidate = true;
	RELEASE_INSTANCE(CManagement);
}
