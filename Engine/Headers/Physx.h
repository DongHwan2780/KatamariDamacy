#pragma once

#ifndef __PHYSX_H__
#define __PHYSX_H__

#include "Base.h"
#include "Obj.h"

BEGIN(Engine)

class CPhysX final : public CBase
{
	DECLARE_SINGLETON(CPhysX)
private:
	CPhysX();
	virtual ~CPhysX() = default;

public:
	HRESULT Initialize_Physx();
	_int	Update_Physx(_double DeltaTime);

public:
	void Set_Gravity(CObj* pObj);
	void CollisionFixed(CObj* pObj, _int iSceneIndex, const _tchar *pLayerTag);
	void CollisionNonFix(CObj* pObj, _int iSceneIndex, const _tchar *pLayerTag);
	PxRigidDynamic* CreateDynamicBall(const PxTransform& transform);
	
public:
	virtual void Free() override;

private:
#pragma region PXMEMBER
	// Foundation�� �����ϴµ� �ʿ��� ����
	PxDefaultAllocator		m_pxAllocator;		// PhysX�� �޸𸮸� �Ҵ����� Scene�� �����ϱ� ���� �����͸� Limits�� ����ؼ� �Ҵ緮�� �ٿ������
	PxDefaultErrorCallback	m_pxErrorCallback;		// �����޽��� ������
	PxFoundation*			m_pxFoundation = NULL;

	// PxFoundation�� �־�� gPhysics�� ������ �� �ִ�.
	PxPhysics*				m_pxPhysics = NULL;

	PxDefaultCpuDispatcher*	m_pxDispatcher = NULL;

	PxScene*				m_pxScene = NULL;		// ���� �ùķ��̼��� �����ϱ� ���� ����
	PxMaterial*				m_pxMaterial = NULL;	
	PxTransform				pxTransform = PxTransform(PxVec3(), PxQuat());
#pragma endregion

};
END

#endif // !__PHYSX_H__
