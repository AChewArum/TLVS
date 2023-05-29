#pragma once

#include "Common.h"
#include <PxPhysicsAPI.h>

namespace TL_Common
{
	class ErrorCallback;
}

namespace TL_Physics
{
	class BaseErrorCallback : public physx::PxDefaultErrorCallback
	{
	public:
		BaseErrorCallback(TL_Common::ErrorCallback* callBack);
		virtual ~BaseErrorCallback();

		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
	
	private:
		TL_Common::ErrorCallback* m_pErrorCallback;
	};
}