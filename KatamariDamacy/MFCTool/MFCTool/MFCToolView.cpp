
// MFCToolView.cpp : CMFCToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
#include "ClientDefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
HWND g_hWnd;
HINSTANCE	g_hInst;
// CMFCToolView]

IMPLEMENT_DYNCREATE(CMFCToolView, CView)

BEGIN_MESSAGE_MAP(CMFCToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCToolView ����/�Ҹ�

CMFCToolView::CMFCToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFCToolView �׸���

void CMFCToolView::OnDraw(CDC* /*pDC*/)
{
	CMFCToolDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pManagement->UpdateTool();

	Invalidate(false);

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMFCToolView �μ�

BOOL CMFCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFCToolView ����

#ifdef _DEBUG
void CMFCToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}


CMFCToolDoc* CMFCToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolDoc)));
	return (CMFCToolDoc*)m_pDocument;
}
#endif //_DEBUG

// CMFCToolView �޽��� ó����


void CMFCToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_hWnd = m_hWnd;
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

	pMain->SetWindowPos(nullptr, 0, 0, 800 + iGapX + 1, 600 + iGapY + 1, SWP_NOMOVE);

	HRESULT hr;
	g_hWnd = AfxGetMainWnd()->GetSafeHwnd();
	//CWnd *pWnd = AfxGetMainWnd();
	//HWND hWnd = pWnd->m_hWnd;

	g_hInst = AfxGetInstanceHandle();

	hr = m_pManagement->Ready_GraphicDevice(g_hWnd, 800, 600, &m_pDevice, &m_pDeviceContext);
	m_pManagement->Initialize_Engine(g_hInst, g_hWnd, END_SCENE);
	//Ready_Prototype_Component();

	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 0));
	pForm->Initialize();

#pragma region Obj_Prototype
	_matrix		ScaleMatrix, RotationMatrix, TranslationMatrix;
	_matrix		ModelPivotMatrix;
	ScaleMatrix = XMMatrixScaling(10.f, 10.f, 10.f);
	RotationMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	ModelPivotMatrix = ScaleMatrix;

	hr = m_pManagement->Add_Prototype(STATIC_SCENE, TEXT("Component_Model_StageMap"), CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/GameObject/StageMap/", "StageMap.fbx", TEXT("../../Client/Bin/ShaderFiles/Shader_Mesh.fx"), ModelPivotMatrix));

	hr = m_pManagement->Add_Prototype(TEXT("GameObject_ToolCamera"), CToolCamera::Create(m_pDevice, m_pDeviceContext));

#pragma endregion

}

HRESULT CMFCToolView::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 5.f, -6.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.TransformDesc.fSpeedPerSec = 5.0f;
	CameraDesc.TransformDesc.fRotatePerSec = XMConvertToRadians(90.0f);

	if (FAILED(m_pManagement->Add_GameObj(STATIC_SCENE, TEXT("GameObject_ToolCamera"), pLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMFCToolView::Ready_Layer_StageMap(const _tchar * pLayerTag)
{
	if (FAILED(m_pManagement->Add_GameObj(STATIC_SCENE, TEXT("GameObject_StageMap"), pLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMFCToolView::Ready_Prototype_Component()
{
	if (m_pManagement == nullptr)
		return E_FAIL;

	/* For.Prototype_Transform */
	if (FAILED(m_pManagement->Add_Prototype(STATIC_SCENE, TEXT("Component_Transform"), CTransform::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Renderer */
	if (FAILED(m_pManagement->Add_Prototype(STATIC_SCENE, TEXT("Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer);

	return S_OK;
}


void CMFCToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMainFrame->m_tMainSplitter.GetPane(0, 0));
	CObjTool*	pObjTool = &pForm->m_tObjTool;

	if (pForm->m_CtrlTab_Main.GetCurSel() == 0)
	{
		Ready_Layer_Camera(TEXT("Layer_Camera"));
		Ready_Layer_StageMap(TEXT("Layer_StageMap"));
	}

	CView::OnLButtonDown(nFlags, point);
}
