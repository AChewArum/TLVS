#pragma once

#include "RigidActor.h"

namespace Physics
{
	/// <summary>
	/// RigidBody Component가 있는 물리 객체에 대응대는 RigidActor
	/// </summary>
	class RigidBody :
		public RigidActor
	{
	public:
		RigidBody();
		~RigidBody();

		virtual bool isKindOf(const char* name);

	private:
		PxRigidDynamic* _pxRigidBody;
	};
}

