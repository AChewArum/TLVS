#pragma once

#include "Common.h"
#include "Math\TL_Math.h"

namespace TL_GameEngine
{
	class Collider;

	/// @brief	Describes a contact point where the collision occurs. Contact points are stored in Collision structure.
	struct ContactPoint
	{
		ContactPoint()
			: _normal{ 0.0f, 0.0f, 0.0f }
			, _point{ 0.0f, 0.0f, 0.0f }
			, _separation{ 0.0f }
			, _otherCollider(nullptr)
			, _thisCollider(nullptr)
		{

		}

		ContactPoint(TL_Math::Vector3 normal, TL_Math::Vector3 point, float separation, 
			Collider* otherCollider, Collider* thisCollider)
			: _normal(normal)
			, _point(point)
			, _separation(separation)
			, _otherCollider(otherCollider)
			, _thisCollider(thisCollider)
			{
			}

		/// @brief Normal of the contact point.
		TL_Math::Vector3 _normal;

		/// @brief 	The point of contact.
		TL_Math::Vector3 _point;

		/// @brief 	The distance between the colliders at the contact point.
		float _separation;

		/// @brief 	The other collider in contact at the point.
		Collider* _otherCollider;

		/// @brief 	The first collider in contact at the point.
		Collider* _thisCollider;
		
	};

}