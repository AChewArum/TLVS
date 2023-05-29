#include "PhysicsPCH.h"
#include "Physics\PhysicsFactory.h"
#include "PhysicsImpl.h"
#include "Physics\Metal\PhysicsMaterial.h"
#include "Physics\PhysicsObject.h"
#include "PhysicsScene.h"

using namespace TL_Physics;

PHYSICS_DLL_DECLSPEC PhysicsScene* TL_Physics::CreatePhysicsScene()
{
	PhysicsScene* _physicsScene = new PhysicsScene(PhysicsSDK->GetPhysics(), PhysicsSDK->GetCpuDispatcher());

	return _physicsScene;
}

PHYSICS_DLL_DECLSPEC void TL_Physics::CreatePhysicsMaterial(uuid uuid, PhysicsMaterialInfo& materialInfo)
{
	PhysicsSDK->CreateMaterial(uuid, materialInfo);
}

PHYSICS_DLL_DECLSPEC PhysicsObject* TL_Physics::CreateCollider(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, PhysicsMaterial* physicsMaterial/*= 0*/)
{
	PhysicsObject* _physicsObject = new PhysicsObject(userData, pose, colliderInfo, physicsMaterial);

	if (_physicsObject->IsVaild())
	{
		return _physicsObject;
	}
	else
	{
		return nullptr;
	}
}

PHYSICS_DLL_DECLSPEC PhysicsObject* TL_Physics::CreateRigidBody(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, RigidInfo& rigidInfo, PhysicsMaterial* physicsMaterial/*= 0*/)
{
	PhysicsObject* _physicsObject = new PhysicsObject(userData, pose, colliderInfo, rigidInfo, physicsMaterial);

	if (_physicsObject->IsVaild())
	{
		return _physicsObject;
	}
	else
	{
		return nullptr;
	}
}

PHYSICS_DLL_DECLSPEC bool TL_Physics::DeletePhysicsObject(TL_Physics::PhysicsObject* physicsObject)
{
	NullCheck(physicsObject);

	delete physicsObject;

	return true;
}
