// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MapTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Form.h"
#include "MFCToolView.h"

#include "ToolTerrain.h"
// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_iTileX(0)
	, m_iTileY(0)
	, m_fTileInterval(0)
	, m_iIndex(0)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_iWidthLength(0)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_iTileX);
	DDX_Text(pDX, IDC_EDIT3, m_iTileY);
	DDX_Text(pDX, IDC_EDIT4, m_fTileInterval);
	DDX_Text(pDX, IDC_EDIT14, m_iIndex);
	DDX_Text(pDX, IDC_EDIT5, m_fPosX);
	DDX_Text(pDX, IDC_EDIT6, m_fPosY);
	DDX_Text(pDX, IDC_EDIT7, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT13, m_iWidthLength);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON10, &CMapTool::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON11, &CMapTool::OnBnClickedButtonRight)
	ON_BN_CLICKED(IDC_BUTTON12, &CMapTool::OnBnClickedButtonDown)

	ON_BN_CLICKED(IDC_BUTTON9, &CMapTool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTool::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapTool::OnBnClickedButtonAll)
	ON_BN_CLICKED(IDC_BUTTON7, &CMapTool::OnBnClickedButtonApplyWidth)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapTool::OnBnClickedButtonApplyLength)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


void CMapTool::OnBnClickedButtonUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iChangeIndex = m_iIndex += m_iTileY;
	m_fPosX = m_pVIBuffer->Get_VertexPos(iChangeIndex).x / m_fTileInterval;
	m_fPosY = m_pVIBuffer->Get_VertexPos(iChangeIndex).y;
	m_fPosZ = m_pVIBuffer->Get_VertexPos(iChangeIndex).z / m_fTileInterval;
	UpdateData(FALSE);
}


void CMapTool::OnBnClickedButtonLeft()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iChangeIndex = m_iIndex -= 1;
	m_fPosX = m_pVIBuffer->Get_VertexPos(iChangeIndex).x / m_fTileInterval;
	m_fPosY = m_pVIBuffer->Get_VertexPos(iChangeIndex).y;
	m_fPosZ = m_pVIBuffer->Get_VertexPos(iChangeIndex).z / m_fTileInterval;
	UpdateData(FALSE);
}


void CMapTool::OnBnClickedButtonRight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iChangeIndex = m_iIndex += 1;
	m_fPosX = m_pVIBuffer->Get_VertexPos(iChangeIndex).x / m_fTileInterval;
	m_fPosY = m_pVIBuffer->Get_VertexPos(iChangeIndex).y;
	m_fPosZ = m_pVIBuffer->Get_VertexPos(iChangeIndex).z / m_fTileInterval;
	UpdateData(FALSE);
}


void CMapTool::OnBnClickedButtonDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iChangeIndex = m_iIndex -= m_iTileY;
	m_fPosX = m_pVIBuffer->Get_VertexPos(iChangeIndex).x / m_fTileInterval;
	m_fPosY = m_pVIBuffer->Get_VertexPos(iChangeIndex).y;
	m_fPosZ = m_pVIBuffer->Get_VertexPos(iChangeIndex).z / m_fTileInterval;
	UpdateData(FALSE);
}


void CMapTool::OnBnClickedButtonSave()
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
	//D:\박병건\119\FrameWork (6) (2)
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	// D:\박병건\119\FrameWork (6) (2)\Data
	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		CVIBuffer_Terrain* m_pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pManagement->GetComponent(STATIC_SCENE, L"Layer_Terrain", L"Com_VIBuffer"));

		_float3 pVertexPos = { 0.f, 0.f, 0.f };

		WriteFile(hFile, &m_iTileX, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &m_iTileY, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &m_fTileInterval, sizeof(_float), &dwByte, nullptr);

		for (_uint i = 0; i < (m_iTileX * m_iTileY) - 1; ++i)
		{
			pVertexPos = m_pTerrainBuffer->Get_VertexPos(i);

			WriteFile(hFile, &pVertexPos, sizeof(_float3), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
	RELEASE_INSTANCE(CManagement);
	pView->m_bInvalidate = true;
}


void CMapTool::OnBnClickedButtonCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CManagement*	pGameInstance = GET_INSTANCE(CManagement);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCToolView* pView = dynamic_cast<CMFCToolView*>(pMain->m_tMainSplitter.GetPane(0, 1));
	/* Component_VIBuffer_Terrain */

//	pGameInstance->Add_Prototype(STATIC_SCENE, TEXT("Component_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(pView->Get_Device(), pView->Get_DeviceContext(), TEXT("../../Client/Bin/ShaderFiles/Shader_Terrain.fx"), m_iTileX, m_iTileY, m_fTileInterval));

	//if (m_iTileX > 0 && m_iTileY > 0)
	//{
	//	pGameInstance->Add_GameObj(STATIC_SCENE, L"GameObject_Terrain", L"Layer_Terrain");
	//}



	RELEASE_INSTANCE(CManagement);

	UpdateData(FALSE);
}


void CMapTool::OnBnClickedButtonApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_pVIBuffer->Set_TerrainPosY(m_iIndex, (m_fPosY / 10.f));
	UpdateData(FALSE);
}


void CMapTool::OnBnClickedButtonAll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	for (_uint i = 0; i < m_iTileY + 1; ++i)
	{
		for (_uint j = 0; j < m_iTileX + 1; ++j)
		{
			iIndex = (m_iTileY * j) + i;
			m_pVIBuffer->Set_TerrainPosY(iIndex, (m_fPosY / 10.f));
		}
	}
	UpdateData(FALSE);
}


void CMapTool::OnBnClickedButtonApplyWidth()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	for (_uint i = m_iIndex; m_iIndex + m_iWidthLength > i; ++i)
	{
		m_pVIBuffer->Set_TerrainPosY(i, (m_fPosY / 10.f));
	}
	UpdateData(FALSE);
}


void CMapTool::OnBnClickedButtonApplyLength()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	for (_uint i = 0; i < m_iWidthLength; ++i)
	{
		m_pVIBuffer->Set_TerrainPosY(m_iIndex + i* m_iTileX, (m_fPosY / 10.f));
	}
	UpdateData(FALSE);
}

