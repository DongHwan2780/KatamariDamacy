#include "stdafx.h"
#include "..\Headers\Apple.h"

#include "Management.h"

CApple::CApple(DEVICES)
	:CObjList(pDevice, pDeviceContext)
{
}

CApple::CApple(const CApple & other)
	: CObjList(other)
{
}

HRESULT CApple::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CApple::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg)
	{
		memcpy(&TransformDesc, pArg, sizeof(CTransform::TRANSFORMDESC));
	}

	m_pTransform->Set_State(CTransform::POSITION, TransformDesc.vPosition);
	m_pTransform->Set_Scale(XMVectorSet(TransformDesc.fScale, TransformDesc.fScale, TransformDesc.fScale, 0.f));
	m_pTransform->Set_TransformDesc(TransformDesc);

	return S_OK;
}

_int CApple::Update(_double DeltaTime)
{
	if (0 > __super::Update(DeltaTime))
		return -1;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	if (m_pColliderSphere->GetColliderDesc().eObjState == CCollider::OBJ_STICK && m_bStickCheck == true)
	{
		m_pPlayerBallTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_PlayerBall", L"Com_Transform"));

		_vector vPos = m_pTransform->Get_State(CTransform::POSITION);
		_vector vBallPos = m_pPlayerBallTransform->Get_State(CTransform::POSITION);

		OffsetMatrix = XMMatrixIdentity();
		OffsetMatrix = XMMatrixTranslationFromVector(vPos - vBallPos);

		// 포지션을 구해주고 구한 포지션에 공의 회전값만큼 공전을 해줘라

		_vector  vScale, vQuat, vTrans;
		XMMatrixDecompose(&vScale, &vQuat, &vTrans, m_pPlayerBallTransform->Get_WorldMatrix());

		OffsetMatrix = OffsetMatrix * XMMatrixRotationQuaternion(vQuat);

		//vRight = OffsetMatrix.r[0];
		//vUp = OffsetMatrix.r[1];
		//vLook = OffsetMatrix.r[2];

		//OffsetMatrix.r[0] = XMVector4Transform(vRight, RotationMatrix);
		//OffsetMatrix.r[1] = XMVector4Transform(vUp, RotationMatrix);
		//OffsetMatrix.r[2] = XMVector4Transform(vLook, RotationMatrix);

		//Set_State(CTransform::RIGHT, XMVector4Transform(vRight, RotationMatrix));
		//_matrix RotXMatrix, RotYMatrix, RotZMatrix;
		//RotXMatrix = OffsetMatrix * XMMatrixTranslationFromVector(m_pPlayerBallTransform->Get_State(CTransform::RIGHT));
		//RotYMatrix = OffsetMatrix * XMMatrixTranslationFromVector(m_pPlayerBallTransform->Get_State(CTransform::UP));
		//RotZMatrix = OffsetMatrix * XMMatrixTranslationFromVector(m_pPlayerBallTransform->Get_State(CTransform::LOOK));
		//OffsetMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&m_pColliderSphere->GetColliderDesc().vPos));
		//OffsetMatrix = XMMatrixTranslationFromVector(XMQuaternionNormalize(vPos) - XMQuaternionNormalize(vBallPos));	
		
		//_matrix			NonRotateMatrix;
		//NonRotateMatrix.r[3] = OffsetMatrix.r[3];

		//_matrix			NonScaleMatrix = NonRotateMatrix;

		//NonRotateMatrix.r[0] = XMVector3Normalize(OffsetMatrix.r[0]);
		//NonRotateMatrix.r[1] = XMVector3Normalize(OffsetMatrix.r[1]);
		//NonRotateMatrix.r[2] = XMVector3Normalize(OffsetMatrix.r[2]);
		_matrix		TransMatrix;
		TransMatrix = XMMatrixTranslation(0.f, 0.5f, 2.f);


		m_pTransform->Set_WorldMatrix(OffsetMatrix * m_pPlayerBallTransform->Get_WorldMatrix());
		m_pTransform->Set_Scale(XMVectorSet(m_pTransform->GetTransformDesc().fScale, m_pTransform->GetTransformDesc().fScale, m_pTransform->GetTransformDesc().fScale, 0.f));

		m_bStickCheck = false;
	}

	m_pColliderSphere->Update_State(m_pTransform->Get_WorldMatrix());

	RELEASE_INSTANCE(CManagement);

	return _int();
}

_int CApple::Late_Update(_double DeltaTime)
{
	if (nullptr == m_pRenderer)
		return -1;

	if (0 > __super::Late_Update(DeltaTime))
		return -1;
	CManagement*	pManagement = GET_INSTANCE(CManagement);
	if (m_pColliderSphere->GetColliderDesc().eObjState == CCollider::OBJ_STICK && m_bStickCheck == false)
	{
		m_pPlayerBallTransform = dynamic_cast<CTransform*>(pManagement->GetComponent(STAGEONE_SCENE, L"Layer_PlayerBall", L"Com_Transform"));

		//_vector vPos = m_pTransform->Get_State(CTransform::POSITION);
		//_vector vBallPos = m_pPlayerBallTransform->Get_State(CTransform::POSITION);

		//if (m_bStickCheck == true)
		//{
		//	OffsetMatrix = XMMatrixTranslationFromVector(vPos - vBallPos);
		//}

		_matrix			NonScaleMatrix = m_pPlayerBallTransform->Get_WorldMatrix();

		NonScaleMatrix.r[0] = XMVector3Normalize(m_pPlayerBallTransform->Get_WorldMatrix().r[0]);
		NonScaleMatrix.r[1] = XMVector3Normalize(m_pPlayerBallTransform->Get_WorldMatrix().r[1]);
		NonScaleMatrix.r[2] = XMVector3Normalize(m_pPlayerBallTransform->Get_WorldMatrix().r[2]);

		// 오브 포지션  - 공 포지션 == 오프셋

		// 오프셋 * 스케일 뺀 공
		_matrix		TransMatrix;
		TransMatrix = XMMatrixTranslation(0.f, 0.5f, 2.f);


		m_pTransform->Set_WorldMatrix(OffsetMatrix * m_pPlayerBallTransform->Get_WorldMatrix());
		m_pTransform->Set_Scale(XMVectorSet(m_pTransform->GetTransformDesc().fScale, m_pTransform->GetTransformDesc().fScale, m_pTransform->GetTransformDesc().fScale, 0.f));
	}
	RELEASE_INSTANCE(CManagement);

	return m_pRenderer->Add_RenderGroup(CRenderer::NONALPHA, this);
}

HRESULT CApple::Render()
{
	if (nullptr == m_pModel)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint		iNumMaterials = (_uint)m_pModel->Get_NumMaterials();

	m_pModel->Bind_Buffers();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModel->Render(i, 0);
	}

#ifdef _DEBUG
	m_pColliderSphere->Render();
#endif

	return S_OK;
}

HRESULT CApple::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Model_Apple", L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	/* For.Com_Transform */

	if (FAILED(__super::SetUp_Components(STATIC_SCENE, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
	ColliderDesc.eObjState = CCollider::OBJ_NONE;
	if (FAILED(__super::SetUp_Components(STAGEONE_SCENE, L"Component_Collider_SPHERE", L"Com_SPHERE", (CComponent**)&m_pColliderSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CApple::SetUp_ConstantTable()
{
	if (nullptr == m_pModel)
		return E_FAIL;

	CManagement*	pManagement = GET_INSTANCE(CManagement);

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pTransform->Get_WorldMatrix()), sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(pManagement->Get_Transform(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(pManagement->Get_Transform(CPipeLine::D3DTS_PROJ)), sizeof(_matrix));

	RELEASE_INSTANCE(CManagement);

	return S_OK;
}

CApple * CApple::Create(DEVICES)
{
	CApple*		pInstance = new CApple(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Creating CApple");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CObj * CApple::Clone(void * pArg)
{
	CApple*		pInstance = new CApple(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CApple");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CApple::Free()
{

	Safe_Release(m_pColliderSphere);
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRenderer);

	__super::Free();
}
