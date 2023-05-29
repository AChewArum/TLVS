#include "PhysicsPCH.h"
#include "PhysicsHelper.h"

physx::PxTransform TL_Physics::TransformToPxTransform(const TransformInfo& value)
{
	physx::PxTransform temp;

	temp.p.x = value._pos.x;
	temp.p.y = value._pos.y;
	temp.p.z = value._pos.z;

	temp.q.x = value._rot.x;
	temp.q.y = value._rot.y;
	temp.q.z = value._rot.z;
	temp.q.w = value._rot.w;

	return temp;
}

TL_Physics::TransformInfo TL_Physics::PxTransformToTransform(const physx::PxTransform& value)
{
	TransformInfo temp;

	temp._pos.x = value.p.x;
	temp._pos.y = value.p.y;
	temp._pos.z = value.p.z;

	temp._rot.x = value.q.x;
	temp._rot.y = value.q.y;
	temp._rot.z = value.q.z;
	temp._rot.w = value.q.w;

	return temp;
}

physx::PxVec3 TL_Physics::Vector3ToPxVec3(const TL_Math::Vector3& value)
{
	physx::PxVec3 temp;

	temp.x = value.x;
	temp.y = value.y;
	temp.z = value.z;

	return temp;
}

TL_Math::Vector3 TL_Physics::PxVec3ToVector3(const physx::PxVec3& value)
{
	TL_Math::Vector3 temp;

	temp.x = value.x;
	temp.y = value.y;
	temp.z = value.z;

	return temp;
}
