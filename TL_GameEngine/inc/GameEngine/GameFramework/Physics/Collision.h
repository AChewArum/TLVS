#pragma once

#include "Common.h"
#include "Math\TL_Math.h"

#include "GameEngine\GameFramework\Physics\ContactPoint.h"

namespace TL_GameEngine
{
	struct ContactPoint;
	class GameObject;
	class RigidBody;
	class Collider;

	/// @brief Collision information is passed to Collider.OnCollisionEnter, Collider.OnCollisionStay and Collider.OnCollisionExit events.
	class Collision
	{
	public:
		Collision();
		virtual ~Collision();

		//////////////////////////////////////////////////////////////////////////
		// Getter & Setter
		//////////////////////////////////////////////////////////////////////////

		inline ContactPoint GetContact(int index);
		inline std::vector<ContactPoint>& GetContacts() { return m_ContactPoints; }
		inline int GetContactCount() { return m_iContactCount; }
		inline RigidBody* GetRigidBody() { return m_pRigidBody; }
		inline Collider* GetCollider() { return m_pCollider; }
		inline GameObject* GetGameObject() { return m_pGameObject; }
		inline TL_Math::Vector3& GetImpulse() { return m_Impulse; }
		inline TL_Math::Vector3& GetRelaticeVelocity() { return m_RelativeVelocity; }

		inline void AddContact(ContactPoint& value) { m_ContactPoints.push_back(value); }
		inline void SetContacts(std::vector<ContactPoint>& values) { m_ContactPoints.assign(values.begin(), values.end()); }
		inline void SetContactCount(int value) { m_iContactCount = value; }
		inline void SetRigidBody(RigidBody* value) { m_pRigidBody = value; }
		inline void SetCollider(Collider* value) { m_pCollider = value; }
		inline void SetGameObject(GameObject* value) { m_pGameObject = value; }
		inline void SetImpulse(const TL_Math::Vector3& value) { m_Impulse = value; }
		inline void SetRelaticeVelocity(const TL_Math::Vector3& value) { m_RelativeVelocity = value; }
	private:
		/// @brief Gets the number of contacts for this collision.
		int m_iContactCount;

		/// @brief The Rigidbody we hit. This is null if the object we hit is a collider with no rigidbody attached.
		RigidBody* m_pRigidBody;

		/// @brief The Collider we hit.
		Collider* m_pCollider;

		/// @brief The GameObject whose collider you are colliding with.
		GameObject* m_pGameObject;
		
		/// @brief The contact points generated by the physics engine. You should avoid using this as it produces memory garbage. Use GetContact or GetContacts instead.
		std::vector<ContactPoint> m_ContactPoints;

		/// @brief 	The total impulse applied to this contact pair to resolve the collision.
		TL_Math::Vector3 m_Impulse;

		/// @brief The relative linear velocity of the two colliding objects.
		TL_Math::Vector3 m_RelativeVelocity;
	};
}