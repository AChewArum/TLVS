#include "GameEngine_pch.h"
#include "GameEngine\GameFramework\Physics\PhysicsErrorCallback.h"

#include "Log/Log.h"

namespace TL_GameEngine
{

	PhysicsErrorCallback::PhysicsErrorCallback()
	{

	}

	PhysicsErrorCallback::~PhysicsErrorCallback()
	{

	}

	void PhysicsErrorCallback::Error(const char* msg)
	{
		Log::Client_Error(msg);
	}

	void PhysicsErrorCallback::Waring(const char* msg)
	{
		Log::Client_Warn(msg);
	}

	void PhysicsErrorCallback::Log(const char* msg)
	{
		Log::Client_Debug(msg);
	}

}