#pragma once
#include "RigidActor.h"

namespace Physics
{
	/// <summary>
	/// RigidBody Component가 없는 물리 객체에 대응하는 RigidActor
	/// </summary>
	class ColliderBody :
		public RigidActor
	{
	public:
		ColliderBody(PxTransform& transform);
		~ColliderBody();

		virtual bool isKindOf(const char* name);

	private:
		PxRigidStatic* _pxRigidStatic;
	};
}

