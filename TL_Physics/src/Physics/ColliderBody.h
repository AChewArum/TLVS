#pragma once
#include "RigidActor.h"

namespace Physics
{
	/// <summary>
	/// RigidBody Component�� ���� ���� ��ü�� �����ϴ� RigidActor
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

