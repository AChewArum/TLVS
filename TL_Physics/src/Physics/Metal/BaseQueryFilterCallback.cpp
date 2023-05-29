#include "..\PhysicsPCH.h"
#include "Physics\Metal\BaseQueryFilterCallback.h"

namespace TL_Physics
{
	physx::PxQueryHitType::Enum BaseQueryFilterCallback::preFilter(const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags)
	{
		
		return physx::PxQueryHitType::Enum();
	}

	physx::PxQueryHitType::Enum BaseQueryFilterCallback::postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit)
	{

		return physx::PxQueryHitType::Enum();
	}
}