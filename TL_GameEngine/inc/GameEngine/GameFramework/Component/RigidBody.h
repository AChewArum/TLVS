#pragma once

#include "Common.h"
#include "GameEngine\Gameframework\Component\ComponentBase.h"
#include "Physics\PhysicsInfo.h"

namespace TL_Physics
{
	class PhysicsObject;
}

namespace TL_GameEngine
{
	class Collider;

	/// @brief pxForceMode
	enum class eForceMode
	{
		Force = 0
		, Impulse = 1
		, VelocityChange = 2
		, Acceleration = 3
	};

	class GAME_ENGINE_API RigidBody : public ComponentBase
	{
	public:
		RigidBody(class GameObject* _gameObject, const tstring& _typeName = TEXT("RigidBody"));
		virtual ~RigidBody();

		//////////////////////////////////////////////////////////////////////////
		// Inherited Component
		//////////////////////////////////////////////////////////////////////////

		virtual void PreTick();

		virtual void PostTick();

		virtual void TryTick();

		virtual void Tick();

		virtual void OnStart();

		virtual void OnChangeEnable(bool _newEnable);

		//////////////////////////////////////////////////////////////////////////
		// Member Function
		//////////////////////////////////////////////////////////////////////////


		/// <summary>
		/// Set rigid actor's global position & rotation
		/// </summary>
		/// <param name="transformInfo">global position & rotation</param>
		void SetTransform(TL_Physics::TransformInfo& transformInfo);

		/// <summary>
		/// Get rigid actor's global position & rotation
		/// </summary>
		/// <returns>global position & rotation</returns>
		TL_Physics::TransformInfo GetTransform();

		//////////////////////////////////////////////////////////////////////////
		// RigidBody Control
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Set rigid body's mass value
		/// </summary>
		/// <param name="mass">mass</param>
		/// <returns>success</returns>
		bool SetMass(float mass);

		/// <summary>
		/// Set rigid body's linear drag
		/// </summary>
		/// <param name="damping">damping</param>
		/// <returns>success</returns>
		bool SetLinearDamping(float damping);

		/// <summary>
		/// Set rigid body's angular drag
		/// </summary>
		/// <param name="damping">angular damping</param>
		/// <returns>success</returns>
		bool SetAngularDamping(float damping);

		/// <summary>
		/// Set rigid body's gravify flag
		/// </summary>
		/// <param name="flag">flag</param>
		/// <returns>success</returns>
		bool SetGravity(bool flag);

		/// <summary>
		/// Set rigid body's kinematic flag
		/// </summary>
		/// <param name="flag">flag</param>
		/// <returns>success</returns>
		bool SetKinematic(bool flag);

		/// <summary>
		/// Set rigid body's axis lock flag
		/// </summary>
		/// <param name="constraints">constraints</param>
		/// <returns>success</returns>
		bool SetConstraints(TL_Physics::Constraints& constraints);

		bool SetLinearVelocity(TL_Math::Vector3 velocity);
		bool SetAngularVelocity(TL_Math::Vector3 velocity);
		bool SetMaxLinearVelocity(float val);
		bool SetMaxAngularVelocity(float val);
		TL_Math::Vector3 GetLinearVelocity();
		TL_Math::Vector3 GetAngularVelocity();

		bool SetCollisionDetection(TL_Physics::eCollisionDetection collisionDetection);

		//////////////////////////////////////////////////////////////////////////
		// External force
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Applying force to a body
		/// </summary>
		/// <param name="vector">linear force</param>
		/// <returns>success</returns>
		bool AddForce(TL_Math::Vector3 vector, eForceMode forceMode = eForceMode::Force);

		/// <summary>
		/// Apply a torque to a rigid body
		/// </summary>
		/// <param name="torque">torque</param>
		/// <returns>success</returns>
		bool AddTorque(TL_Math::Vector3 torque, eForceMode forceMode = eForceMode::Force);



		//////////////////////////////////////////////////////////////////////////
		// Getter & Setter
		//////////////////////////////////////////////////////////////////////////

		inline TL_Physics::RigidInfo& GetRigidInfo() { return m_RigidInfo; }
		inline void SetRigidInfo(TL_Physics::RigidInfo& rigidInfo) { m_RigidInfo = rigidInfo; }

		inline Collider* GetCollider() { return m_pCollider; }

		inline TL_Physics::PhysicsObject* GetPhysicsObject() { return m_pPhysicsObject; }
		inline void SetPhysicsObject(TL_Physics::PhysicsObject* value) { m_pPhysicsObject = value; }

	private:
		TL_Physics::RigidInfo m_RigidInfo;

		Collider* m_pCollider;

		TL_Physics::PhysicsObject* m_pPhysicsObject;
	};
}