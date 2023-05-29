#include "GameEngine_pch.h"
#include "GameEngine\GameFramework\Component\Collider.h"
#include "GameEngine\GameFramework\Component\RigidBody.h"
#include "GameEngine\GameFramework\Component\IPhysicsEventComponent.h"
#include "GameEngine\Gameframework\GameObject.h"

#include "GameEngine\GameFramework\Physics\PhysicsSystem.h"

#include "GameEngine\GameFramework\Physics\Collision.h"

#include "Physics\PhysicsObject.h"

namespace TL_GameEngine
{
	Collider::Collider(GameObject* _gameObject, const tstring& _typeName, TL_Physics::ColliderInfo& _colliderInfo)
		: ComponentBase(_gameObject, _typeName)
		, m_ColliderInfoRef(_colliderInfo)
	{

	}

	Collider::~Collider()
	{
		Physics.DeletePhysicsObject(m_pPhysicsObject);
	}

	void Collider::FixedTick()
	{
		TL_Physics::TransformInfo _transformInfo{ m_pTransform->GetWorldPosition(), m_pTransform->GetWorldRotation() };

		m_pPhysicsObject->SetTransform(_transformInfo);

		Physics.AddPhysicsMessage(this);
	}

	void Collider::PreTick()
	{

	}

	void Collider::PostTick()
	{

	}

	void Collider::TryTick()
	{

	}

	void Collider::Tick()
	{
		TL_Math::Vector3 _currScale = m_pTransform->GetWorldScale();

		if (m_PrevScale != _currScale)
		{
			m_IsDirty = true;
			m_PrevScale = _currScale;
		}

		if (m_ColliderInfoRef._isTrigger)
		{
			OnTriggerStayMessage();
		}
	}

	void Collider::OnStart()
	{
		m_pTransform = m_GameObject->GetComponent<Transform>();
		m_pAttachedRigidBody = m_GameObject->GetComponent<RigidBody>();
		
		m_IsAttachedRigidBody = (m_pAttachedRigidBody != nullptr);

		// Todo : name, uid 아직 확정이 안된듯 보임
		TL_Physics::UserData _userData{m_GameObject->GetName().c_str(), 0, m_GameObject
		, m_pAttachedRigidBody, this};

		TL_Physics::TransformInfo _transformInfo{ m_pTransform->GetWorldPosition(), m_pTransform->GetWorldRotation(), m_pTransform->GetWorldScale() };

		if (m_IsAttachedRigidBody)
		{
			m_pPhysicsObject = Physics.CreateRigidBodyObject(_userData, _transformInfo, m_ColliderInfoRef, m_pAttachedRigidBody->GetRigidInfo(), m_pPhysicsMaterial);
		
			m_pAttachedRigidBody->SetPhysicsObject(m_pPhysicsObject);
		}
		else
		{
			m_pPhysicsObject = Physics.CreateColliderObject(_userData, _transformInfo, m_ColliderInfoRef, m_pPhysicsMaterial);
		}

		m_pPhysicsObject->SetLayer(m_GameObject->GetLayer());


		/*
		* Todo : 이벤트 콜백을 하기 위한 임시 방편 만약 게임오브젝트 쪽에서
		* 컴포넌트별로 따로 short cut를 가지게 되면 거기서 처리하면 된다.
		*/

		for (auto* _component : m_GameObject->GetAllComponents())
		{
			if (_component->CheckFlag(ComponentFlag::eCOLLISION))
			{
				auto* _cast = dynamic_cast<ICollisionComponent*>(_component);

				m_CollisionComponents.push_back(_cast);
			}
			if (_component->CheckFlag(ComponentFlag::eTRIGGER))
			{
				auto* _cast = dynamic_cast<ITriggerComponent*>(_component);

				m_TriggerComponents.push_back(_cast);
			}
		}

		OnChangeEnable(m_GameObject->GetEnable() && m_bEnable);
	}

	void Collider::OnChangeEnable(bool _newEnable)
	{
		_newEnable ? m_pPhysicsObject->OnEnable() : m_pPhysicsObject->OnDisable();
	}

	void Collider::AttachRigidBody(RigidBody* _rigidBody)
	{
		m_IsAttachedRigidBody = true;
		m_pAttachedRigidBody = _rigidBody;
	}

	void Collider::DetachRigidBody()
	{
		m_IsAttachedRigidBody = false;
		m_pAttachedRigidBody = nullptr;
	}

	void Collider::FetchResult()
	{
		if (!m_IsAttachedRigidBody)
			return;

		auto _simulateResult = m_pPhysicsObject->GetTransform();

		m_pTransform->SetPositionAndRotation(_simulateResult._pos, _simulateResult._rot);
	}

	void Collider::OnCollisionEnterMessage(Collision& collision)
	{
		for (auto& component : m_CollisionComponents)
		{
			component->OnCollisionEnter(collision);
		}
	}

	void Collider::OnCollisionStayMessage(Collision& collision)
	{
		foreach(iter, ICollisionComponent*, m_CollisionComponents)
		{
			(*iter)->OnCollisionStay(collision);
		}
	}

	void Collider::OnCollisionExitMessage(Collision& collision)
	{
		foreach(iter, ICollisionComponent*, m_CollisionComponents)
		{
			(*iter)->OnCollisionExit(collision);
		}
	}

	void Collider::OnTriggerEnterMessage(Collider& collider)
	{
		m_OtherColliders.push_back(&collider);

		foreach(iter, ITriggerComponent*, m_TriggerComponents)
		{
			(*iter)->OnTriggerEnter(collider);
		}
	}

	void Collider::OnTriggerStayMessage()
	{
		foreach(colliderIter, Collider*, m_OtherColliders)
		{
			foreach(componentIter, ITriggerComponent*, m_TriggerComponents)
			{
				(*componentIter)->OnTriggerStay(**colliderIter);
			}
		}
	}

	void Collider::OnTriggerExitMessage(Collider& collider)
	{
		const auto _iter = std::ranges::find(m_OtherColliders, &collider);
		assert(_iter != m_OtherColliders.end());

		m_OtherColliders.erase(_iter);

		foreach(iter, ITriggerComponent*, m_TriggerComponents)
		{
			(*iter)->OnTriggerExit(collider);
		}
	}

	RigidBody* Collider::GetAttachRigidBody()
	{
		return m_pAttachedRigidBody;
	}

	UID Collider::GetMaterialUID()
	{
		return m_PhysicsMaterialUID;
	}

	void Collider::SetMaterial(TL_Physics::PhysicsMaterial* _material)
	{
		m_pPhysicsMaterial = _material;

		if (m_pPhysicsObject != nullptr)
		{
			m_pPhysicsObject->SetPhysicsMaterial(_material);
		}
	}

	TL_Physics::PhysicsMaterial* Collider::GetMaterial()
	{
		return m_pPhysicsMaterial;
	}

	void Collider::SetTrigger(bool flag)
	{
		m_ColliderInfoRef._isTrigger = flag;

		if(m_pPhysicsObject != nullptr)
		{
			m_pPhysicsObject->SetIsTrigger(flag);
		}
	}

	bool Collider::GetTrigger()
	{
		return m_ColliderInfoRef._isTrigger;
	}
}