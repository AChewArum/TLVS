#pragma once

#include <PxQueryFiltering.h>

namespace TL_Physics
{
	class BaseQueryFilterCallback : public physx::PxQueryFilterCallback
	{
	public:
		BaseQueryFilterCallback() = default;
		~BaseQueryFilterCallback() = default;

		physx::PxQueryHitType::Enum preFilter(
			const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) override;

		physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit) override;

	};
}