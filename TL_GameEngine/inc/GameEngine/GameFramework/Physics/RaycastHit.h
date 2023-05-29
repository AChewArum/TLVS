#pragma once

#include "Common.h"

#include "Math\TL_Math.h"

namespace TL_GameEngine
{
	class RigidBody;
	class Collider;
	class Transform;

	struct RaycastHit
	{
		RaycastHit()
		: _barcentricCoordinate{ 0.f, 0.f, 0.f }
		, _rigidBody(nullptr)
		, _collider(nullptr)
		, _distance(0.0f)
		, _lightmapCoord{ 0.f, 0.f }
		, _normal{ 0.f, 0.f, 0.f }
		, _point{ 0.f, 0.f, 0.f }
		, _textureCoord{ 0.f, 0.f }
		, _textureCoord2{ 0.f, 0.f }
		, _transform(nullptr)
		, _triangleIndex(0)
		{}
		RaycastHit(RigidBody* rigidBody, Collider* collider, float distance, TL_Math::Vector3& normal, TL_Math::Vector3& point, Transform* transform, uint64 faceIndex)
		: _rigidBody(rigidBody)
		, _collider(collider)
		, _distance(distance)
		, _normal(normal)
		, _point(point)
		, _transform(transform)
		, _triangleIndex(faceIndex)
		{
		}

		TL_Math::Vector3 _barcentricCoordinate;	// don't use
		RigidBody* _rigidBody;
		Collider* _collider;
		float _distance;
		TL_Math::Vector2 _lightmapCoord;		// don't use
		TL_Math::Vector3 _normal;
		TL_Math::Vector3 _point;
		TL_Math::Vector2 _textureCoord;			// don't use
		TL_Math::Vector2 _textureCoord2;		// don't use
		Transform* _transform;
		uint64 _triangleIndex;					// don't use
	};
}