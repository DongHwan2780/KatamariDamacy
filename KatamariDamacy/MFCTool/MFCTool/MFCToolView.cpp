
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
#include "ToolBag.h"
#include "ToolBanana.h"
#include "ToolBat.h"
#include "ToolBansai.h"
#include "ToolBook0.h"
#include "ToolBook1.h"
#include "ToolBowling.h"
#include "ToolCake.h"
#include "ToolCan.h"
#include "ToolCandy.h"
#include "ToolClock.h"
#include "ToolDenden.h"
#include "ToolDice.h"
#include "ToolDrinkCan0.h"
#include "ToolDrinkCan1.h"
#include "ToolEgg.h"
#include "ToolEraser.h"
#include "ToolFork.h"
#include "ToolFryPan.h"
#include "ToolLipStick.h"
#include "ToolMagnet.h"
#include "ToolMelon.h"
#include "ToolMilkPack.h"
#include "ToolOrange.h"
#include "ToolPilon.h"
#include "ToolPin0.h"
#include "ToolPin1.h"
#include "ToolRadio.h"
#include "ToolSpoon.h"
#include "ToolStand.h"
#include "ToolSunFlower.h"
#include "ToolTree.h"
#include "ToolTulip.h"

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

	ScaleMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	RotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	ModelPivotMatrix = ScaleMatrix * RotationMatrix;
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Apple", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Apple.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Bag", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Bag.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Banana", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Banana.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Bat", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Bat.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Bansai", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Bonsai.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Book0", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Book0.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Book1", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Book1.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Bowling", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Bowling.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Cake", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Cake.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Can", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Can.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Candy", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Candy.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Clock", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Clock.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Denden", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Denden.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Dice", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Dice.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_DrinkCan0", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "DrinkCan0.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_DrinkCan1", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "DrinkCan1.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Egg", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Egg.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Eraser", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Eraser.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Fork", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Fork.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_FryPan", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "FryPan.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_LipStick", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "LipStick.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Magnet", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Magnet.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Melon", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Melon.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_MilkPack", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "MilkPack.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Orange", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Orange.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Pilon", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Pilon.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Pin0", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Pin0.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Pin1", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Pin1.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Radio", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Radio.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Spoon", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Spoon.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Stand", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Stand.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_SunFlower", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "SunFlower.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Tree", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Tree.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));
	hr = m_pManagement->Add_Prototype(STATIC_SCENE, L"Component_Model_Tulip", CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/Object/", "Tulip.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));



	hr = m_pManagement->Add_Prototype(L"GameObject_ToolCamera", CToolCamera::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_StageMap", CToolMap::Create(m_pDevice, m_pDeviceContext));

	hr = m_pManagement->Add_Prototype(L"GameObject_Apple",		CToolApple::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Bag",		CToolBag::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Banana",		CToolBanana::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Bat",		CToolBat::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Bansai",		CToolBansai::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Book0",		CToolBook0::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Book1",		CToolBook1::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Bowling",	CToolBowling::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Cake",		CToolCake::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Can",		CToolCan::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Candy",		CToolCandy::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Clock",		CToolClock::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Denden",		CToolDenden::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Dice",		CToolDice::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_DrinkCan0",	CToolDrinkCan0::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_DrinkCan1",	CToolDrinkCan1::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Egg",		CToolEgg::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Eraser",		CToolEraser::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Fork",		CToolFork::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_FryPan",		CToolFryPan::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_LipStick",	CToolLipStick::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Magnet",		CToolMagnet::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Melon",		CToolMelon::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_MilkPack",	CToolMilkPack::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Orange",		CToolOrange::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Pilon",		CToolPilon::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Pin0",		CToolPin0::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Pin1",		CToolPin1::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Radio",		CToolRadio::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Spoon",		CToolSpoon::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Stand",		CToolStand::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_SunFlower",	CToolSunFlower::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Tree",		CToolTree::Create(m_pDevice, m_pDeviceContext));
	hr = m_pManagement->Add_Prototype(L"GameObject_Tulip",		CToolTulip::Create(m_pDevice, m_pDeviceContext));
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
