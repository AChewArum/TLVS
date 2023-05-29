#include "pch.h"
#include "ColliderBody.h"
#include "PhysicsImpl.h"

Physics::ColliderBody::ColliderBody(PxTransform& transform)
{
	_pxRigidStatic = PhysicsSDK->GetPhysics()->createRigidStatic(transform);
}

Physics::ColliderBody::~ColliderBody()
{
	SAFE_RELEASE(_pxRigidStatic)
}

bool Physics::ColliderBody::isKindOf(const char* name)
{

}