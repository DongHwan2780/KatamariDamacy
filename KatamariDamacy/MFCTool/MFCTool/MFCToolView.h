
// MFCToolView.h : CMFCToolView 클래스의 인터페이스
//

#pragma once

#include "Management.h"
#include "ClientDefines.h"

class CMFCToolDoc;

class CMFCToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCToolView();
	DECLARE_DYNCREATE(CMFCToolView)

// 특성입니다.
public:
	CMFCToolDoc* GetDocument() const;
	CManagement* m_pManagement;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	CRenderer*				m_pRenderer = nullptr;
	CVIBuffer_Terrain*		m_pVIBuffer = nullptr;
	_float4x4			matWorld;

	_bool		m_bInvalidate = true;
	_uint		m_iVertexIndex = 0;
private:
	_bool		m_bFirst = true;
// 작업입니다.
public:
	ID3D11Device* Get_Device() const { return m_pDevice; }
	ID3D11DeviceContext* Get_DeviceContext() const { return m_pDeviceContext; }
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();

	HRESULT Ready_Layer_Camera(const wstring& pLayerTag);
	HRESULT Ready_Layer_StageMap(const wstring& pLayerTag);
	HRESULT Ready_Prototype_Component();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFCToolView.cpp의 디버그 버전
inline CMFCToolDoc* CMFCToolView::GetDocument() const
   { return reinterpret_cast<CMFCToolDoc*>(m_pDocument); }
#endif