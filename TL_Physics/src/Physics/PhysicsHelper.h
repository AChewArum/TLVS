#pragma once

namespace TL_Physics
{
	physx::PxTransform TransformToPxTransform(const TransformInfo& value);
	TransformInfo PxTransformToTransform(const physx::PxTransform& value);

	physx::PxVec3 Vector3ToPxVec3(const TL_Math::Vector3& value);
	TL_Math::Vector3 PxVec3ToVector3(const physx::PxVec3& value);
}