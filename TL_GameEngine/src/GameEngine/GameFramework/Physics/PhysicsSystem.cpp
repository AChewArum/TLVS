#include "GameEngine_pch.h"
#include "GameEngine\Gameframework\Physics\PhysicsSystem.h"
#include "GameEngine\Gameframework\Physics\Collision.h"
#include "GameEngine\Gameframework\Physics\ContactPoint.h"
#include "GameEngine\Gameframework\Physics\RaycastHit.h"
#include "GameEngine\GameFramework\Physics\PhysicsErrorCallback.h"

#include "GameEngine\GameFramework\GameWorld.h"
#include "GameEngine\GameFramework\GameObject.h"
#include "GameEngine\Gameframework\Component\IPhysicsEventComponent.h"
#include "GameEngine\Gameframework\Component\Collider.h"
#include "GameEngine\Gameframework\Component\RigidBody.h"

#include "GameEngine\internal\UUIDGenerator.h"

#include "Physics\IPhysicsImpl.h"
#include "Physics\IPhysicsScene.h"
#include "Physics\PhysicsInfo.h"
#include "Physics\PhysicsFactory.h"
#include "Physics\PhysicsObject.h"
#include "Physics\Metal\PhysicsMaterial.h"


PhysicsConstructor physicsConstructor;
PhysicsDestructor physicsDestructor;
HMODULE g_physicsDLL;
static TL_GameEngine::PhysicsSystem* g_pInstance = nullptr;

uint32 DefaultRaycastLayers = UINT32_MAX;

namespace TL_GameEngine
{
	PhysicsSystem::PhysicsSystem()
		: m_pPhysicsImpl(nullptr)
		, m_PhysicsInfo()
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{
		physicsDestructor();
	}

	PhysicsSystem& PhysicsSystem::Instance()
	{
		if (g_pInstance == nullptr)
		{
			g_pInstance = new PhysicsSystem();
		}

		return *g_pInstance;
	}

	void PhysicsSystem::Release()
	{
		SAFE_DELETE(g_pInstance);
		FreeLibrary(g_physicsDLL);
	}

	void PhysicsSystem::Initalize(PhysicsInfo& physicsSetting)
	{
		if(!LoadPhysicsDLL())
			AssertMessageBox(false, "Physics DLL load failed");

		m_pPhysicsImpl = physicsConstructor();

		// ErrorCallback 생성
		m_PhysicsErrorCallback = std::make_shared<PhysicsErrorCallback>();
		m_pPhysicsImpl->SetErrorCallback(m_PhysicsErrorCallback.get());

		// Physics 초기화
		m_pPhysicsImpl->Initalize();

		m_PhysicsInfo = physicsSetting;

		m_pPhysicsImpl->PhysicsSetting(physicsSetting);


		InitPhysicsLayer();
	}

	void PhysicsSystem::Simulate(float elapsedTime)
	{
		m_pPhysicsImpl->Simulate(elapsedTime);
	}

	void PhysicsSystem::Tick(float deltaTime)
	{
		m_CurrFramePerCollider = 0;

		// Todo : Time class 추후에 추가시 변경 해야함
		const float fixedTime = 0.01666666667f;

		bool simulateEnd = false;
		m_fFixedTimer += deltaTime;

		m_CurrFrameColliders.reserve(m_MaxFramePerCollider);

		while (m_fFixedTimer >= fixedTime)
		{
			m_fFixedTimer -= fixedTime;

			FixedUpdateMessage();

			Simulate(fixedTime);

			FetchSimulateResult();

			OnTriggerEventMessage();
			OnCollisionEventMessage();
			WaitForFiexedUpdateMessage();
		}

		if (m_CurrFramePerCollider > m_MaxFramePerCollider)
		{
			m_MaxFramePerCollider = m_CurrFramePerCollider;
		}

		m_CurrFrameColliders.clear();
	}

	TL_Physics::PhysicsObject* PhysicsSystem::CreateColliderObject(TL_Physics::UserData userData, TL_Physics::TransformInfo& pose, TL_Physics::ColliderInfo& colliderInfo, TL_Physics::PhysicsMaterial* physicsMaterial)
	{
		return m_pPhysicsImpl->CreateCollider(userData, pose, colliderInfo, physicsMaterial);
	}

	TL_Physics::PhysicsObject* PhysicsSystem::CreateRigidBodyObject(TL_Physics::UserData userData, TL_Physics::TransformInfo& pose, TL_Physics::ColliderInfo& colliderInfo, TL_Physics::RigidInfo& rigidInfo, TL_Physics::PhysicsMaterial* physicsMaterial)
	{
		return m_pPhysicsImpl->CreateRigidBody(userData, pose, colliderInfo, rigidInfo, physicsMaterial);
	}

	bool PhysicsSystem::DeletePhysicsObject(TL_Physics::PhysicsObject* physicsObject)
	{
		return m_pPhysicsImpl->DeletePhysicsObject(physicsObject);
	}

	void PhysicsSystem::AddPhysicsMessage(Collider* collider)
	{
		m_CurrFramePerCollider++;

		m_CurrFrameColliders.push_back(collider);
	}

	void PhysicsSystem::AddTrigger(TriggerComponentBase* trigger)
	{
		SAFE_INSERT_MAP(trigger, m_TriggerComponentBases, trigger->GetUUID());
	}

	void PhysicsSystem::AddCollisionEvent(CollisionComponentBase* collision)
	{
		SAFE_INSERT_MAP(collision, m_CollisionComponentBases, collision->GetUUID());
	}

	void PhysicsSystem::DeleteTriggerEvent(TriggerComponentBase* trigger)
	{
		SAFE_DELETE_MAP(m_TriggerComponentBases, trigger->GetUUID());
	}

	void PhysicsSystem::DeleteCollisionEvent(CollisionComponentBase* collision)
	{
		SAFE_DELETE_MAP(m_CollisionComponentBases, collision->GetUUID());
	}

	TL_Physics::PhysicsMaterial* PhysicsSystem::CreatePhysicsMaterial(TL_Physics::PhysicsMaterialInfo& materialInfo)
	{
		UUIDGenerator uuid;

		m_pPhysicsImpl->CreateMaterial(uuid.Generate(), materialInfo);

		return nullptr;
	}

	void PhysicsSystem::SetLayer(int i, Layer physicsLayer)
	{
		TL_Physics::PhysicsLayer _layer;

		_layer._word0 = 1 << i;

		_layer._word1 = physicsLayer;

		m_pPhysicsImpl->SetFilter(i, _layer);
	}
	bool PhysicsSystem::Raycast(TL_Math::Vector3& orgin, TL_Math::Vector3& direction, float maxDistance, uint32 layerMask)
	{
		bool _ret;

		_ret = m_pPhysicsImpl->Raycast(orgin, direction, maxDistance, layerMask);

		return _ret;
	}

	bool PhysicsSystem::Raycast(TL_Math::Vector3& orgin, TL_Math::Vector3& direction, RaycastHit& hitinfo, float maxDistance, uint32 layerMask)
	{
		bool _ret;

		TL_Physics::RaycastHitInfo _raycastHitInfo;

		_ret = m_pPhysicsImpl->Raycast(orgin, direction, _raycastHitInfo, maxDistance, layerMask);
		if (_ret == true)
		{
			hitinfo = {
			static_cast<RigidBody*>(_raycastHitInfo._userData->_pRigidBody),
			static_cast<Collider*>(_raycastHitInfo._userData->_pCollider),
			_raycastHitInfo._distance,
			_raycastHitInfo._normal,
			_raycastHitInfo._point,
			static_cast<GameObject*>(_raycastHitInfo._userData->_pObject)->GetComponent<Transform>(),
			_raycastHitInfo._faceIndex
			};

			hitinfo._barcentricCoordinate = { _raycastHitInfo._u, _raycastHitInfo._v };
		}

		return _ret;
	}

	bool PhysicsSystem::GetRenderBufferPoints(std::vector<DebugPoint>& points)
	{
		return m_pPhysicsImpl->GetRenderBufferPoints(points);
	}

	bool PhysicsSystem::GetRenderBufferLines(std::vector<DebugLine>& lines)
	{
		return m_pPhysicsImpl->GetRenderBufferLines(lines);
	}

	bool PhysicsSystem::GetRenderBufferTriangles(std::vector<DebugTriangle>& triangles)
	{
		return m_pPhysicsImpl->GetRenderBufferTriangles(triangles);
	}

	void PhysicsSystem::InitPhysicsLayer()
	{
		for (int i = 0; i < 32; i++)
		{
			TL_Physics::PhysicsLayer _layer;

			_layer._word0 = 1 << i;
			
			_layer._word1 = UINT32_MAX;

			m_pPhysicsImpl->AddFilter(_layer);
		}
	}

	bool PhysicsSystem::LoadPhysicsDLL()
	{
		g_physicsDLL = LoadLibrary(_T("TL_Physics.dll"));

		AssertMessageBox(g_physicsDLL != NULL, "Load physics dll failed !");

		physicsConstructor = (PhysicsConstructor)GetProcAddress(g_physicsDLL, "PhysicsImplInstance");

		AssertMessageBox(physicsConstructor != NULL, "Load physics dll failed !");

		physicsDestructor = (PhysicsDestructor)GetProcAddress(g_physicsDLL, "PhysicsImplRelease");

		AssertMessageBox(physicsDestructor != NULL, "Load physics dll failed !");

		return true;
	}

	void PhysicsSystem::FixedUpdateMessage()
	{
		GameWorld::GetInstance()->FixedTick();
	}

	void PhysicsSystem::OnTriggerEventMessage()
	{
		auto& events = m_pPhysicsImpl->GetTriggerEvents();

		for (const auto& triggerEvent : events)
		{
			auto* _collider1 = reinterpret_cast<Collider*>(triggerEvent._userData1->_pCollider);
			auto* _collider2 = reinterpret_cast<Collider*>(triggerEvent._userData2->_pCollider);

			AssertMessageBox(_collider1 != nullptr && _collider2 != nullptr, "Error trigger event data");

			switch (triggerEvent._eventType)
			{
				case eEventType::Enter:
				{
					_collider1->OnTriggerEnterMessage(*_collider2);
					_collider2->OnTriggerEnterMessage(*_collider1);
					break;
				}
				case eEventType::Exit:
				{
					_collider1->OnTriggerExitMessage(*_collider2);
					_collider2->OnTriggerExitMessage(*_collider1);
					break;
				}
			}
		}
	}

	void PhysicsSystem::OnCollisionEventMessage()
	{
		auto& events = m_pPhysicsImpl->GetCollisionEvents();

		for (const auto& collisionEvent : events)
		{
			Collision _collision1;
			Collision _collision2;

			MakeCollision(_collision1, collisionEvent, collisionEvent._userData1, collisionEvent._userData2);
			MakeCollision(_collision2, collisionEvent, collisionEvent._userData2, collisionEvent._userData1);

			auto* _collider1 = reinterpret_cast<Collider*>(collisionEvent._userData1->_pCollider);
			auto* _collider2 = reinterpret_cast<Collider*>(collisionEvent._userData2->_pCollider);

			switch (collisionEvent._eventType)
			{
				case eEventType::Enter:
				{
					_collider1->OnCollisionEnterMessage(_collision2);
					_collider2->OnCollisionEnterMessage(_collision1);
					break;
				}
				case eEventType::Stay:
				{
					_collider1->OnCollisionStayMessage(_collision2);
					_collider2->OnCollisionStayMessage(_collision1);
					break;
				}
				case eEventType::Exit:
				{
					_collider1->OnCollisionExitMessage(_collision2);
					_collider2->OnCollisionExitMessage(_collision1);
					break;
				}
			}
		}
	}

	void PhysicsSystem::WaitForFiexedUpdateMessage()
	{

	}

	void PhysicsSystem::FetchSimulateResult()
	{
		for (auto& collider : m_CurrFrameColliders)
		{
			collider->FetchResult();
		}
	}

	void PhysicsSystem::MakeCollision(Collision& collision, const TL_Physics::CollisionEvent& event, TL_Physics::UserData* thisData, TL_Physics::UserData* otherData)
	{
		collision.SetContactCount(event._contactCount);

		collision.SetGameObject((GameObject*)otherData->_pObject);

		collision.SetCollider((Collider*)otherData->_pCollider);

		collision.SetRigidBody((RigidBody*)otherData->_pRigidBody);

		collision.SetImpulse(event._impulse);

		for (auto& _point : event._contactPoints)
		{
			ContactPoint _contactPoint{_point._normal, _point._point, _point._separation, reinterpret_cast<Collider*>(otherData->_pCollider), reinterpret_cast<Collider*>(thisData->_pCollider) };

			collision.AddContact(_contactPoint);
		}
	}

}