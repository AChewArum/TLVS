#include "PhysicsPCH.h"
#include "Physics\IPhysicsImpl.h"
#include "PhysicsImpl.h"

//static std::shared_ptr<IPhysicsImpl> g_physicsImpl = nullptr;
static TL_Physics::IPhysicsImpl* g_physicsImpl = nullptr;

//PHYSICS_DLL_DECLSPEC std::shared_ptr<IPhysicsImpl> Physics::PhysicsImplInstance()
//{
//	if (g_physicsImpl == nullptr)
//	{
//		g_physicsImpl = std::make_shared<Physics::PhysicsImpl>();
//		g_physicsImpl->Initalize();
//	}
//
//	return g_physicsImpl;
//}

PHYSICS_DLL_DECLSPEC IPhysicsImpl* TL_Physics::PhysicsImplInstance()
{
	if (g_physicsImpl == nullptr)
	{
		g_physicsImpl = PhysicsImpl::Instance();
	}

	return g_physicsImpl;
}

PHYSICS_DLL_DECLSPEC void TL_Physics::PhysicsImplRelease()
{
	g_physicsImpl->Finalize();

	PhysicsImpl::Release();

	g_physicsImpl = nullptr;
}
