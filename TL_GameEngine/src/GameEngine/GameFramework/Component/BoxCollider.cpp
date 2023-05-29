#include "GameEngine_pch.h"
#include "GameEngine\GameFramework\Component\BoxCollider.h"
#include "GameEngine\GameFramework\GameObject.h"

#include "Physics\PhysicsObject.h"

namespace TL_GameEngine
{
	BoxCollider::BoxCollider(GameObject* _gameObject, const tstring& _typeName)
		:Collider(_gameObject, _typeName, m_BoxColliderInfo)
	{
	}

	BoxCollider::~BoxCollider()
	{
	}

	void BoxCollider::PreTick()
	{
		__super::PreTick();
	}

	void BoxCollider::Tick()
	{
		__super::Tick();

		if (m_IsDirty)
		{
			m_pPhysicsObject->SetSize(m_BoxColliderInfo._size * m_PrevScale);
			m_pPhysicsObject->SetCenter(m_BoxColliderInfo._center);
			m_IsDirty = false;
		}
	}

	void BoxCollider::PostTick()
	{
		__super::PostTick();
	}

	void BoxCollider::TryTick()
	{
		__super::TryTick();
	}

	void BoxCollider::OnStart()
	{
		__super::OnStart();
	}

	void BoxCollider::OnChangeEnable(bool _newEnable)
	{
		__super::OnChangeEnable(_newEnable);
	}

	inline void BoxCollider::SetCenter(float x, float y, float z)
	{
		m_IsDirty = true;

		m_BoxColliderInfo._center = { x, y, z };
	}

	inline void BoxCollider::SetCenter(TL_Math::Vector3 center)
	{
		m_IsDirty = true;

		m_BoxColliderInfo._center = center;
	}

	inline TL_Math::Vector3& BoxCollider::GetCenter()
	{
		return m_BoxColliderInfo._center;
	}

	inline void BoxCollider::SetSize(float x, float y, float z)
	{
		m_IsDirty = true;

		m_BoxColliderInfo._size = { x, y, z };

		//m_pPhysicsObject->SetSize(m_BoxColliderInfo._size);
	}

	inline void BoxCollider::SetSize(TL_Math::Vector3 size)
	{
		m_IsDirty = true;

		m_BoxColliderInfo._size = size;

		//m_pPhysicsObject->SetSize(m_BoxColliderInfo._size);
	}

	inline TL_Math::Vector3& BoxCollider::GetSize()
	{
		return m_BoxColliderInfo._size;
	}

}