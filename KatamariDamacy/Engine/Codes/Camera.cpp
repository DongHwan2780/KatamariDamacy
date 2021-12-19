#include "..\Headers\Camera.h"

#include "Transform.h"
#include "PipeLine.h"

CCamera::CCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CObj(pDevice, pDeviceContext)
	, m_pPipeline(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pPipeline);
}

CCamera::CCamera(const CCamera & other)
	:CObj(other)
	, m_pPipeline(other.m_pPipeline)
{
	Safe_AddRef(m_pPipeline);
}

HRESULT CCamera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	m_pTransform = CTransform::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pTransform->Set_TransformDesc(m_CameraDesc.TransformDesc);

	_vector	vLook = XMLoadFloat3(&m_CameraDesc.vAt) - XMLoadFloat3(&m_CameraDesc.vEye);
	m_pTransform->Set_State(CTransform::LOOK, XMVector3Normalize(vLook));

	_vector	vRight = XMVector3Cross(XMLoadFloat3(&m_CameraDesc.vAxisY), vLook);
	m_pTransform->Set_State(CTransform::RIGHT, XMVector3Normalize(vRight));

	_vector	vUp = XMVector3Cross(vLook, vRight);
	m_pTransform->Set_State(CTransform::UP, XMVector3Normalize(vUp));

	m_pTransform->Set_State(CTransform::POSITION, XMLoadFloat3(&m_CameraDesc.vEye));

	return S_OK;
}

_int CCamera::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	m_pPipeline->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransform->Get_WorldMatrixInverse());
	m_pPipeline->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), _float(1280) / 720.0f, 0.2f, 300.f));

	return _int();
}

_int CCamera::Late_Update(_double DeltaTime)
{
	if (0 > __super::Late_Update(DeltaTime))
		return -1;

	return _int();
}

HRESULT CCamera::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pPipeline);
	Safe_Release(m_pTransform);
}
