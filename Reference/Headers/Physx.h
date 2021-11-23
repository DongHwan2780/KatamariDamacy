#pragma once

#ifndef __PHYSX_H__
#define __PHYSX_H__

#include "Base.h"

BEGIN(Engine)

class CPhysX final : public CBase
{
private:
	CPhysX();
	virtual ~CPhysX() = default;

public:
	HRESULT Initialize_Physx();
	
public:
	virtual void Free() override;

private:
#pragma region PXMEMBER
	// Foundation을 생성하는데 필요한 변수
	PxDefaultAllocator		m_pxAllocator;		// PhysX의 메모리를 할당해줌 Scene을 구성하기 전에 데이터를 Limits을 사용해서 할당량을 줄여줘야함
	PxDefaultErrorCallback	m_pxErrorCallback;		// 에러메시지 보여줌
	PxFoundation*			m_pxFoundation = NULL;

	// PxFoundation이 있어야 gPhysics를 생성할 수 있다.
	PxPhysics*				m_pxPhysics = NULL;

	PxDefaultCpuDispatcher*	m_pxDispatcher = NULL;

	PxScene*				m_pxScene = NULL;		// 물리 시뮬레이션을 실험하기 위한 공간
	PxMaterial*				m_pxMaterial = NULL;	
#pragma endregion

};
END

#endif // !__PHYSX_H__
