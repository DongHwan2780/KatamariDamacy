#include "..\Headers\Physx.h"

CPhysX::CPhysX()
{
}

HRESULT CPhysX::Initialize_Physx()
{
	m_pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_pxAllocator, m_pxErrorCallback);

	m_pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pxFoundation, PxTolerancesScale(), true);


	PxSceneDesc sceneDesc(m_pxPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // �߷� ����
													
	m_pxDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pxDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	m_pxScene = m_pxPhysics->createScene(sceneDesc);		// ���� ����


	m_pxMaterial = m_pxPhysics->createMaterial(0.5f, 0.5f, 0.6f); // �浹ü ������,Dynamic������, ź����
	PxRigidStatic* groundPlane = PxCreatePlane(*m_pxPhysics, PxPlane(0, 1, 0, 0), *m_pxMaterial);		// m_pxMaterial ����?�� ���� ���� ���� ����
	m_pxScene->addActor(*groundPlane);	// ���� ������ ������ü ����

	return S_OK;
}

void CPhysX::Free()
{
}
