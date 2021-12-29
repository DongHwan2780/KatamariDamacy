
// MFCToolView.cpp : CMFCToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCTool.h"
#endif

#include "MFCToolDoc.h"
#include "MFCToolView.h"
#include "MainFrm.h"
#include "Form.h"
#include "ObjTool.h"

#include "ToolCamera.h"
#include "ToolMap.h"

#include "ToolApple.h"

#ifdef _MFCTOOL
#define new DEBUG_NEW
#endif

HWND g_hWnd;
HINSTANCE	g_hInst;
// CMFCToolView]

IMPLEMENT_DYNCREATE(CMFCToolView, CView)

BEGIN_MESSAGE_MAP(CMFCToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCToolView 생성/소멸

CMFCToolView::CMFCToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCToolView::~CMFCToolView()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);
	CManagement::Release_Engine();
}

BOOL CMFCToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCToolView 그리기

void CMFCToolView::OnDraw(CDC* /*pDC*/)
{
	CMFCToolDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//m_pManagement->UpdateTool();

	m_pManagement->Update_Tool(1.0 / 40.0);

	m_pManagement->Update_Scene(1.0 / 40.0);

	m_pManagement->Clear_BackBufferView(_float4(0.f, 0.f, 1.f, 1.f));
	m_pManagement->Clear_DepthStencilView(1.f, 0);

	m_pRenderer->Draw_RenderGroup();

	m_pManagement->Render_Scene();

	m_pManagement->Present();

	Invalidate(false);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFCToolView 인쇄

BOOL CMFCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCToolView 진단

#ifdef _DEBUG
void CMFCToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}


CMFCToolDoc* CMFCToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolDoc)));
	return (CMFCToolDoc*)m_pDocument;
}
#endif //_DEBUG

// CMFCToolView 메시지 처리기


void CMFCToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_hWnd = m_hWnd;
	g_hInst = AfxGetApp()->m_hInstance;

	m_pManagement = CManagement::GetInstance();
	Safe_AddRef(m_pManagement);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	RECT rcMain{};

	pMain->GetWindowRect(&rcMain);
	rcMain.right = rcMain.right - rcMain.left;
	rcMain.bottom = rcMain.bottom - rcMain.top;
	rcMain.left = 0;
	rcMain.top = 0;

	RECT rcView{};
	GetClientRect(&rcView);
	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, g_iWinCX + iGapX + 1, g_iWinCY + iGapY + 1, SWP_NOMOVE);

	HRESULT hr;
	
	hr = m_pManagement->Ready_GraphicDevice(g_hWnd, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDeviceContext);
	m_pManagement->Initialize_Engine_Tool(g_hInst, g_hWnd, END_SCENE);
	Ready_Prototype_Component();

	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 0));
	pForm->Initialize();

#pragma region Obj_Prototype
	_matrix		ScaleMatrix, RotationMatrix, TranslationMatrix;
	_matrix		ModelPivotMatrix;
	ScaleMatrix = XMMatrixScaling(15.f, 15.f, 15.f);
	RotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	ModelPivotMatrix = ScaleMatrix;

	// 맵
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_StageMap", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/StageMap/", "StageMap.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));

	ScaleMatrix = XMMatrixScaling(100.f, 100.f, 100.f);
	RotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	ModelPivotMatrix = ScaleMatrix ;
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Apple", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Apple.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));

	hr = m_pManagement->Add_Prototype(L"GameObject_ToolCamera", CToolCamera::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_StageMap", CToolMap::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Apple", CToolApple::Create(m_pDevice, m_pDeviceContext));

#pragma endregion
}

HRESULT CMFCToolView::Ready_Layer_Camera(const wstring& pLayerTag)
{
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 5.f, -6.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.TransformDesc.fSpeedPerSec = 5.0f;
	CameraDesc.TransformDesc.fRotatePerSec = XMConvertToRadians(90.0f);

	if (FAILED(m_pManagement->Add_GameObj(STATIC_SCENE, L"GameObject_ToolCamera", pLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMFCToolView::Ready_Layer_StageMap(const wstring& pLayerTag)
{
	if (FAILED(m_pManagement->Add_GameObj(STATIC_SCENE, L"GameObject_StageMap", pLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMFCToolView::Ready_Prototype_Component()
{
	if (m_pManagement == nullptr)
		return E_FAIL;

	/* For.Prototype_Transform */
	if (FAILED(m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Transform", CTransform::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Renderer */
	if (FAILED(m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer);

	return S_OK;
}


void CMFCToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMainFrame->m_tMainSplitter.GetPane(0, 0));
	CObjTool*	pObjTool = &pForm->m_tObjTool;

	if (pForm->m_CtrlTab_Main.GetCurSel() == 0 )
	{
		CModel*		pModel = dynamic_cast<CModel*>(m_pManagement->GetComponent(STATIC_SCENE, L"Layer_StageMap", L"Com_Model"));
		Safe_AddRef(pModel);

		CTransform*	pTransformMap = nullptr;
		CTransform*	pTransformCamera = nullptr;

		_float3 vOut;
		_float3 vPosition;
		_vector vCameraPos = XMVectorZero();

		if (m_bFirst)
		{
			Ready_Layer_Camera(L"Layer_Camera");
			Ready_Layer_StageMap(L"Layer_StageMap");

			pTransformMap = dynamic_cast<CTransform*>(m_pManagement->GetComponent(STATIC_SCENE, L"Layer_StageMap", L"Com_Transform"));
			XMStoreFloat4x4(&matWorld, pTransformMap->Get_WorldMatrix());

			m_bFirst = false;
		}

		else if (pModel->RayCast(vOut, g_hWnd, g_iWinCX, g_iWinCY, matWorld, vCameraPos))
		{
			vPosition = {vOut.x, 0.f, vOut.z};
			//vPosition = vOut;

			CString FbxName;
			_uint iIdx = pObjTool->m_FBXListBox.GetCurSel();
			pObjTool->m_FBXListBox.GetText(iIdx, FbxName);

			m_pManagement->Add_GameObj(STATIC_SCENE, pObjTool->m_strObjName.GetString(), pObjTool->m_strLayerName.GetString(), &vPosition);

			_uint ObjCnt = pObjTool->m_ObjList.GetCount();
			CString ObjListName;
			ObjListName.Format(FbxName + L"%d", ObjCnt);
			pObjTool->m_ObjList.AddString(ObjListName);

			Safe_Release(pModel);
		}
	}

	CView::OnLButtonDown(nFlags, point);
}
