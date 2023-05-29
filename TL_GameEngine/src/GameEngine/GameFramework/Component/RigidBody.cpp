#include "GameEngine_pch.h"
#include "GameEngine\Gameframework\Component\RigidBody.h"
#include "GameEngine\Gameframework\Component\Collider.h"
#include "GameEngine\Gameframework\GameObject.h"

#include "Physics\PhysicsObject.h"

namespace TL_GameEngine
{
	RigidBody::RigidBody(GameObject* _gameObject, const tstring& _typeName)
	: ComponentBase(_gameObject, _typeName)
		, m_RigidInfo()
		, m_pPhysicsObject(nullptr)
		, m_pCollider(nullptr)
	{

	}

	RigidBody::~RigidBody()
	{

	}

	void RigidBody::PreTick()
	{
	}

	void RigidBody::PostTick()
	{
	}

	void RigidBody::TryTick()
	{
	}

	void RigidBody::Tick()
	{
	}

	void RigidBody::OnStart()
	{
		m_pCollider = m_GameObject->GetComponent<Collider>();


	}

	void RigidBody::OnChangeEnable(bool _newEnable)
	{

	}

	void RigidBody::SetTransform(TL_Physics::TransformInfo& transformInfo)
	{
		assert(m_pPhysicsObject != nullptr);

		m_pPhysicsObject->SetTransform(transformInfo);
	}

	TL_Physics::TransformInfo RigidBody::GetTransform()
	{
		assert(m_pPhysicsObject != nullptr);

		return m_pPhysicsObject->GetTransform();
	}

	bool RigidBody::SetMass(float mass)
	{
		assert(m_pPhysicsObject != nullptr);

		m_RigidInfo._mass = mass;

		return m_pPhysicsObject->SetMass(m_RigidInfo._mass);
	}

	bool RigidBody::SetLinearDamping(float damping)
	{
		assert(m_pPhysicsObject != nullptr);

		m_RigidInfo._drag = damping;

		return m_pPhysicsObject->SetLinearDamping(m_RigidInfo._drag);
	}

	bool RigidBody::SetAngularDamping(float damping)
	{
		assert(m_pPhysicsObject != nullptr);

		m_RigidInfo._angularDrag = damping;

		return m_pPhysicsObject->SetAngularDamping(m_RigidInfo._angularDrag);
	}

	bool RigidBody::SetGravity(bool flag)
	{
		assert(m_pPhysicsObject != nullptr);

		m_RigidInfo._useGravity = flag;

		return m_pPhysicsObject->SetGravity(m_RigidInfo._useGravity);
	}

	bool RigidBody::SetKinematic(bool flag)
	{
		assert(m_pPhysicsObject != nullptr);

		m_RigidInfo._isKinematic = flag;

		return m_pPhysicsObject->SetKinematic(m_RigidInfo._isKinematic);
	}

	bool RigidBody::SetConstraints(TL_Physics::Constraints& constraints)
	{
		assert(m_pPhysicsObject != nullptr);

		m_RigidInfo._constraints = constraints;

		return m_pPhysicsObject->SetConstraints(m_RigidInfo._constraints);
	}

	bool RigidBody::SetLinearVelocity(TL_Math::Vector3 velocity)
	{
		assert(m_pPhysicsObject != nullptr);

		return m_pPhysicsObject->SetLinearVelocity(velocity);
	}

	bool RigidBody::SetAngularVelocity(TL_Math::Vector3 velocity)
	{
		assert(m_pPhysicsObject != nullptr);

		return m_pPhysicsObject->SetAngularVelocity(velocity);
	}

	bool RigidBody::SetMaxLinearVelocity(float val)
	{
		assert(m_pPhysicsObject != nullptr);

		return m_pPhysicsObject->SetMaxLinearVelocity(val);
	}

	bool RigidBody::SetMaxAngularVelocity(float val)
	{
		assert(m_pPhysicsObject != nullptr);

		return m_pPhysicsObject->SetMaxAngularVelocity(val);
	}

	TL_Math::Vector3 RigidBody::GetLinearVelocity()
	{
		assert(m_pPhysicsObject != nullptr);

		return m_pPhysicsObject->GetLinearVelocity();
	}

	TL_Math::Vector3 RigidBody::GetAngularVelocity()
	{
		assert(m_pPhysicsObject != nullptr);

		return m_pPhysicsObject->GetAngularVelocity();
	}

	bool RigidBody::SetCollisionDetection(TL_Physics::eCollisionDetection collisionDetection)
	{
		assert(m_pPhysicsObject != nullptr);

		m_RigidInfo._collisionDetection = collisionDetection;

		return m_pPhysicsObject->SetCollisionDetection(collisionDetection);
	}

	bool RigidBody::AddForce(TL_Math::Vector3 vector, eForceMode forceMode)
	{
		assert(m_pPhysicsObject != nullptr);

		return m_pPhysicsObject->AddForce(vector, (int)forceMode);
	}

	bool RigidBody::AddTorque(TL_Math::Vector3 torque, eForceMode forceMode)
	{
		assert(m_pPhysicsObject != nullptr);

		return m_pPhysicsObject->AddTorque(torque, (int)forceMode);;
	}
}