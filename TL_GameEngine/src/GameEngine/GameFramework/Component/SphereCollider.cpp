#include "GameEngine_pch.h"
#include "GameEngine\GameFramework\Component\SphereCollider.h"

namespace TL_GameEngine
{
	SphereCollider::SphereCollider(GameObject* _gameObject, const tstring& _typeName)
		: Collider(_gameObject, _typeName, m_SphereColliderInfo)
	{

	}

	SphereCollider::~SphereCollider()
	{

	}

	void SphereCollider::PreTick()
	{
		__super::PreTick();
	}

	void SphereCollider::Tick()
	{
		__super::Tick();
	}

	void SphereCollider::PostTick()
	{
		__super::PostTick();
	}

	void SphereCollider::TryTick()
	{
		__super::TryTick();
	}

	void SphereCollider::OnStart()
	{
		__super::OnStart();
	}

	void SphereCollider::OnChangeEnable(bool _newEnable)
	{
		__super::OnChangeEnable(_newEnable);
	}

	inline void SphereCollider::SetCenter(float x, float y, float z)
	{
		m_IsDirty = true;

		m_SphereColliderInfo._center = { x, y, z };
	}

	inline void SphereCollider::SetCenter(TL_Math::Vector3 center)
	{
		m_IsDirty = true;

		m_SphereColliderInfo._center = center;
	}

	inline TL_Math::Vector3 SphereCollider::GetCenter()
	{
		return m_SphereColliderInfo._center;
	}

	inline void SphereCollider::SetRadius(float radius)
	{
		m_IsDirty = true;

		m_SphereColliderInfo._radius = radius;
	}

	inline float SphereCollider::GetRadius()
	{
		return m_SphereColliderInfo._radius;
	}
}
