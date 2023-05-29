#include "GameEngine_pch.h"
#include "GameEngine\Gameframework\Component\CapsuleCollider.h"

namespace TL_GameEngine
{
	CapsuleCollider::CapsuleCollider(GameObject* _gameObject, const tstring& _typeName)
		: Collider(_gameObject, _typeName, m_CapsuleColliderInfo)
	{

	}

	CapsuleCollider::~CapsuleCollider()
	{

	}

	void CapsuleCollider::PreTick()
	{
		__super::PreTick();
	}

	void CapsuleCollider::Tick()
	{
		__super::Tick();
	}

	void CapsuleCollider::PostTick()
	{
		__super::PostTick();
	}

	void CapsuleCollider::TryTick()
	{
		__super::TryTick();
	}

	void CapsuleCollider::OnStart()
	{
		__super::OnStart();
	}

	void CapsuleCollider::OnChangeEnable(bool _newEnable)
	{
		__super::OnChangeEnable(_newEnable);
	}

	inline void CapsuleCollider::SetCenter(float _x, float _y, float _z)
	{
		m_IsDirty = true;

		m_CapsuleColliderInfo._center = { _x, _y, _z };
	}

	inline void CapsuleCollider::SetCenter(TL_Math::Vector3 _center)
	{
		m_IsDirty = true;

		m_CapsuleColliderInfo._center = _center;
	}

	inline TL_Math::Vector3 CapsuleCollider::GetCenter()
	{
		return m_CapsuleColliderInfo._center;
	}

	inline void CapsuleCollider::SetRadius(float _radius)
	{
		m_IsDirty = true;

		m_CapsuleColliderInfo._radius = _radius;
	}

	inline float CapsuleCollider::GetRadius()
	{
		return m_CapsuleColliderInfo._radius;
	}

	inline void CapsuleCollider::SetHeight(float _height)
	{
		m_IsDirty = true;

		m_CapsuleColliderInfo._height = _height;
	}

	inline float CapsuleCollider::GetHeight()
	{
		return m_CapsuleColliderInfo._height;
	}

	inline void CapsuleCollider::SetDirection(int _direction)
	{
		m_IsDirty = true;

		m_CapsuleColliderInfo._axisDirection = static_cast<TL_Physics::eAxisDirection>(_direction);
	}

	inline int CapsuleCollider::GetDirection()
	{
		return static_cast<int>(m_CapsuleColliderInfo._axisDirection);
	}
}