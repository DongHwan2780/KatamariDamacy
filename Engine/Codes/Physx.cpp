#include "..\Headers\Physx.h"

IMPLEMENT_SINGLETON(CPhysX)

CPhysX::CPhysX()
{
}

HRESULT CPhysX::Initialize_Physx()
{
	m_pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_pxAllocator, m_pxErrorCallback);


	gPvd = PxCreatePvd(*m_pxFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pxFoundation, PxTolerancesScale(), true, gPvd);
	PxSceneDesc sceneDesc(m_pxPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // 중력 세팅
													
	m_pxDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pxDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	m_pxScene = m_pxPhysics->createScene(sceneDesc);		// 공간 생성

	//PxPvdSceneClient* pvdClient = m_pxScene->getScenePvdClient();
	//if (pvdClient)
	//{
	//	pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	//	pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
	//	pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	//}

	m_pxMaterial = m_pxPhysics->createMaterial(0.5f, 0.5f, 0.6f); // 충돌체 마찰력,Dynamic마찰력, 탄성력
	PxRigidStatic* groundPlane = PxCreatePlane(*m_pxPhysics, PxPlane(0, 1, 0, 0), *m_pxMaterial);		// m_pxMaterial 재질?을 가진 지형 정보 생성
	m_pxScene->addActor(*groundPlane);	// 지형 정보로 지형객체 생성

	//m_pxMaterial = m_pxPhysics->createMaterial(0.2f, 0.2f, 0.6f);

	//PxShape*	pxShape = m_pxPhysics->createShape(PxSphereGeometry(2.f), *m_pxMaterial);
	//pxShape->setFlag(PxShapeFlag::eVISUALIZATION, true);
	//pxShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	//pxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	//pxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);



	return S_OK;
}

_int CPhysX::Update_Physx(_double DeltaTime)
{
	m_pxScene->simulate(DeltaTime);
	m_pxScene->fetchResults(true);

	return _int();
}

void CPhysX::Set_Gravity(CObj * pObj)
{
}

void CPhysX::CollisionFixed(CObj * pObj, _int iSceneIndex, const _tchar * pLayerTag)
{
}

void CPhysX::CollisionNonFix(CObj * pObj, _int iSceneIndex, const _tchar * pLayerTag)
{
}

PxRigidDynamic * CPhysX::CreateDynamicBall(const PxTransform& transform)
{
	PxSphereGeometry	pxSphereGeo = 2.f;
	PxRigidDynamic*		pxActor;

	pxActor = PxCreateDynamic(*m_pxPhysics, transform, pxSphereGeo, *m_pxMaterial, 10.f);
	pxActor->setLinearVelocity(PxVec3(0.f, 0.f, -10.f ));
	//PxTransform a =  pxActor->getGlobalPose();
	//a.p;
	//a.q;

	//m_pTransform->Setposition(a.p);

	m_pxScene->addActor(*pxActor);

	return pxActor;
}

void CPhysX::Free()
{
	PX_RELEASE(m_pxScene);
	PX_RELEASE(m_pxDispatcher);
	PX_RELEASE(m_pxPhysics);
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(m_pxFoundation);
}
