#include "PhysicsPCH.h"
#include "PhysicsImpl.h"
#include "PhysicsScene.h"

#include "Metal\BaseEventCallBack.h"
#include "Metal\BaseErrorCallback.h"
#include "Physics\Metal\PhysicsMaterial.h"
#include "Physics\PhysicsObject.h"

const char* g_pvd_host = "127.0.0.1";
static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;

static TL_Physics::PhysicsImpl* g_physxSDKImpl = nullptr;

TL_Physics::PhysicsImpl::PhysicsImpl()
: m_pPhysics(nullptr)
, m_pFoundation(nullptr)
, m_pCooking(nullptr)
, m_pPvd(nullptr)
, m_pDefaultCpuDispatcher(nullptr)
, m_pCurrScene(nullptr)
, m_BaseErrorCallback(nullptr)
{

}

TL_Physics::PhysicsImpl::~PhysicsImpl()
{
}

TL_Physics::PhysicsImpl* TL_Physics::PhysicsImpl::Instance()
{
	if (g_physxSDKImpl == nullptr)
	{
		g_physxSDKImpl = new PhysicsImpl;
	}

	return g_physxSDKImpl;
}

void TL_Physics::PhysicsImpl::Release()
{
	if (g_physxSDKImpl != nullptr)
	{
		delete g_physxSDKImpl;
	}
}

bool TL_Physics::PhysicsImpl::Initalize()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		*m_BaseErrorCallback);

	NullCheck(m_pFoundation);

 #ifdef _DEBUG
 	m_pPvd = PxCreatePvd(*m_pFoundation);

 	NullCheck(m_pPvd);

 	m_pPvdTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 1000);
 	//_pxPvdTransport = physx::PxDefaultPvdFileTransportCreate("Test.pvd");

 	NullCheck(m_pPvdTransport);

	m_bConnectPvd = m_pPvd->connect(*m_pPvdTransport, physx::PxPvdInstrumentationFlag::eALL);

 	if (!m_bConnectPvd)
 	{
		// MessageBoxRet(ret, "Pvd 연결 실패", MB_CANCELTRYCONTINUE);
		//
		// if (ret == 2)
		// {
		// 	AssertMessageBox(true, "실행 중단함");
		// }
		// else if (ret == 10)
		// {
		// 	m_bConnectPvd = m_pPvd->connect(*m_pPvdTransport, physx::PxPvdInstrumentationFlag::eALL);
		//
		// 	if (!m_bConnectPvd)
		// 	{
		// 		DebugMessageBox("Pvd 재연결 실패");
		// 	}
		// }
 	}
 #endif // _DEBUG
	
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation,
		physx::PxTolerancesScale(), m_IsRecordMemoryAllocations, m_pPvd);
	
	NullCheck(m_pPhysics);

	m_pDefaultCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);

	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));

	NullCheck(m_pCooking);

	m_pCurrScene = new PhysicsScene(m_pPhysics, m_pDefaultCpuDispatcher);

	PhysicsMaterialInfo _defualtInfo;

	m_pDefaultMaterial = new PhysicsMaterial(_defualtInfo);

	return true;
}

void TL_Physics::PhysicsImpl::Finalize()
{
	delete m_pCurrScene;

	delete m_pDefaultMaterial;

	m_pCooking->release();

 #if defined(DEBUG) || defined(_DEBUG)
	if (m_bConnectPvd)
	{
		m_pPvd->disconnect();

		m_pPvdTransport->disconnect();
	}
 #endif

	m_pDefaultCpuDispatcher->release();

	m_pPhysics->release();

 #if defined(DEBUG) || defined(_DEBUG)
 	m_pPvd->release();

 	m_pPvdTransport->release();
 #endif

	m_pFoundation->release();

	if (m_BaseErrorCallback != nullptr)
	{
		delete m_BaseErrorCallback;
	}

}

void TL_Physics::PhysicsImpl::PhysicsSetting(PhysicsInfo& physicInfo)
{
	m_pCurrScene->InitScene(physicInfo);
}

void TL_Physics::PhysicsImpl::Simulate(float dt)
{
	NullCheck(m_pCurrScene);

	m_pCurrScene->Simulate(dt);
}

bool TL_Physics::PhysicsImpl::Raycast(TL_Math::Vector3 origin, TL_Math::Vector3 unitDir, RaycastHitInfo& reycastHitInfo, float maxDistance, uint32 layerMask)
{
	PxVec3 _orgin(origin.x, origin.y, origin.z);
	PxVec3 _unitDir(unitDir.x, unitDir.y, unitDir.z);

	PxRaycastBuffer _buffer;

	bool _ret = m_pCurrScene->Raycast(_orgin, _unitDir, maxDistance, _buffer, layerMask);

	if (_ret)
	{
		TL_Math::Vector3 _normal{ _buffer.block.normal.x, _buffer.block.normal.y, _buffer.block.normal.z };
		TL_Math::Vector3 _point{ _buffer.block.position.x, _buffer.block.position.y, _buffer.block.position.z };

		reycastHitInfo = { reinterpret_cast<UserData*>(_buffer.block.actor->userData),
			_buffer.block.distance,
			_normal,
			_point,
			_buffer.block.u,
			_buffer.block.v,
			_buffer.block.faceIndex };
	}

	return _ret;
}

bool TL_Physics::PhysicsImpl::Raycast(TL_Math::Vector3 origin, TL_Math::Vector3 unitDir, float maxDistance, uint32 layerMask)
{
	PxVec3 _orgine(origin.x, origin.y, origin.z);
	PxVec3 _unitDir(unitDir.x, unitDir.y, unitDir.z);

	PxRaycastBuffer _buffer;

	return m_pCurrScene->Raycast(_orgine, _unitDir, maxDistance, _buffer, layerMask);
}

TL_Physics::PhysicsMaterial* TL_Physics::PhysicsImpl::CreateMaterial(uuid uuid, PhysicsMaterialInfo& materialInfo)
{
	PhysicsMaterial* _material = new PhysicsMaterial(uuid, materialInfo);

	return _material;
}

PhysicsObject* TL_Physics::PhysicsImpl::CreateCollider(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, PhysicsMaterial* physicsMaterial)
{
	PhysicsObject* _physicsObject = new PhysicsObject(userData, pose, colliderInfo, physicsMaterial);

	if (_physicsObject->IsVaild())
	{
		return _physicsObject;
	}
	else
	{
		delete _physicsObject;

		return nullptr;
	}
}

PhysicsObject* TL_Physics::PhysicsImpl::CreateRigidBody(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, RigidInfo& rigidInfo, PhysicsMaterial* physicsMaterial)
{
	PhysicsObject* _physicsObject = new PhysicsObject(userData, pose, colliderInfo, rigidInfo, physicsMaterial);

	if (_physicsObject->IsVaild())
	{
		return _physicsObject;
	}
	else
	{
		delete _physicsObject;

		return nullptr;
	}
}

bool TL_Physics::PhysicsImpl::DeletePhysicsObject(TL_Physics::PhysicsObject* physicsObject)
{
	NullCheck(physicsObject);

	delete physicsObject;

	return true;
}

std::vector<CollisionEvent>& TL_Physics::PhysicsImpl::GetCollisionEvents()
{
	return m_pCurrScene->GetCollisionEvents();
}

std::vector<TriggerEvent>& TL_Physics::PhysicsImpl::GetTriggerEvents()
{
	return m_pCurrScene->GetTriggerEvents();
}

void TL_Physics::PhysicsImpl::AddFilter(PhysicsLayer& layer)
{
	AssertMessageBox(m_PhysicsLayers.size() < 32, "Layer can't add more than 32");

	m_PhysicsLayers.push_back(layer);
}

void TL_Physics::PhysicsImpl::SetFilter(int i, PhysicsLayer& layer)
{
	AssertMessageBox((size_t)i < m_PhysicsLayers.size(), "Layer is not exist");

	m_PhysicsLayers[i] = layer;
}

PhysicsLayer& TL_Physics::PhysicsImpl::GetFilter(int i)
{
	if ((size_t)i >= m_PhysicsLayers.size())
	{
		AssertMessageBox(false, "Layer is not exist");
		return m_PhysicsLayers[0];
	}

	return m_PhysicsLayers[i];
}

bool TL_Physics::PhysicsImpl::GetRenderBufferPoints(std::vector<DebugPoint>& points)
{
	const auto& _renderBuffer = m_pCurrScene->GetScene()->getRenderBuffer();
	
	uint32 _count = _renderBuffer.getNbPoints();

	if (_count == 0)
	{
		return false;
	}

	points.resize(_count);

	memcpy(points.data(), _renderBuffer.getPoints(), sizeof(DebugPoint) * _count);

	/*auto* _currPoint = _renderBuffer.getPoints();

	for (size_t i = 0; i < _count; i++)
	{
		points.push_back(DebugPoint(PxVec3ToVector3(_currPoint->pos), _currPoint->color));

		_currPoint++;
	}*/

	return true;
}

bool TL_Physics::PhysicsImpl::GetRenderBufferLines(std::vector<DebugLine>& lines)
{
	const auto& _renderBuffer = m_pCurrScene->GetScene()->getRenderBuffer();

	uint32 _count = _renderBuffer.getNbLines();

	if (_count == 0)
	{
		return false;
	}

	//const auto* _currPoint = _renderBuffer.getLines();

	//auto* test = reinterpret_cast<const DebugLine*>(_currPoint);

	lines.resize(_count);

	memcpy(lines.data(), _renderBuffer.getLines(), sizeof(DebugLine) * _count);

	/*for (size_t i = 0; i < _count; i++)
	{
		lines.push_back(DebugLine(PxVec3ToVector3(_currPoint->pos0), PxVec3ToVector3(_currPoint->pos1), _currPoint->color0));

		_currPoint++;
	}*/

	return true;
}

bool TL_Physics::PhysicsImpl::GetRenderBufferTriangles(std::vector<DebugTriangle>& triangles)
{
	const auto& _renderBuffer = m_pCurrScene->GetScene()->getRenderBuffer();

	uint32 _count = _renderBuffer.getNbTriangles();

	if (_count == 0)
	{
		return false;
	}

	//auto* _currTriangle = _renderBuffer.getTriangles();

	triangles.resize(_count);

	memcpy(triangles.data(), _renderBuffer.getTriangles(), sizeof(DebugTriangle) * _count);

	/*for (size_t i = 0; i < _count; i++)
	{
		triangles.push_back(DebugTriangle(PxVec3ToVector3(_currTriangle->pos0), PxVec3ToVector3(_currTriangle->pos1), PxVec3ToVector3(_currTriangle->pos2), _currTriangle->color0));

		_currTriangle++;
	}*/

	return true;
}

bool TL_Physics::PhysicsImpl::SetErrorCallback(TL_Common::ErrorCallback* errorCallback)
{
	if (m_BaseErrorCallback == nullptr)
	{
		m_BaseErrorCallback = new BaseErrorCallback(errorCallback);

		return true;
	}
	
	return false;
}
