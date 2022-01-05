
// MFCToolView.h : CMFCToolView Ŭ������ �������̽�
//

#pragma once

#include "Management.h"
#include "ClientDefines.h"

class CMFCToolDoc;

class CMFCToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFCToolView();
	DECLARE_DYNCREATE(CMFCToolView)

// Ư���Դϴ�.
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
// �۾��Դϴ�.
public:
	ID3D11Device* Get_Device() const { return m_pDevice; }
	ID3D11DeviceContext* Get_DeviceContext() const { return m_pDeviceContext; }
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMFCToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();

	HRESULT Ready_Layer_Camera(const wstring& pLayerTag);
	HRESULT Ready_Layer_StageMap(const wstring& pLayerTag);
	HRESULT Ready_Prototype_Component();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFCToolView.cpp�� ����� ����
inline CMFCToolDoc* CMFCToolView::GetDocument() const
   { return reinterpret_cast<CMFCToolDoc*>(m_pDocument); }
#endif