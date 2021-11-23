#include "..\Headers\Physx.h"

CPhysX::CPhysX()
{
}

HRESULT CPhysX::Initialize_Physx()
{
	m_pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_pxAllocator, m_pxErrorCallback);

	m_pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pxFoundation, PxTolerancesScale(), true);


	PxSceneDesc sceneDesc(m_pxPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // 중력 세팅
													
	m_pxDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pxDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	m_pxScene = m_pxPhysics->createScene(sceneDesc);		// 공간 생성


	m_pxMaterial = m_pxPhysics->createMaterial(0.5f, 0.5f, 0.6f); // 충돌체 마찰력,Dynamic마찰력, 탄성력
	PxRigidStatic* groundPlane = PxCreatePlane(*m_pxPhysics, PxPlane(0, 1, 0, 0), *m_pxMaterial);		// m_pxMaterial 재질?을 가진 지형 정보 생성
	m_pxScene->addActor(*groundPlane);	// 지형 정보로 지형객체 생성

	return S_OK;
}

void CPhysX::Free()
{
}
