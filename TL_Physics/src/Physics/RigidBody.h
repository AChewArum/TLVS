#pragma once

#include "RigidActor.h"

namespace Physics
{
	/// <summary>
	/// RigidBody Component�� �ִ� ���� ��ü�� ������� RigidActor
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

