#include "GameEngine_pch.h"
#include "GameEngine\Gameframework\Component\IPhysicsEventComponent.h"

#include "GameEngine\Gameframework\Physics\PhysicsSystem.h"

namespace TL_GameEngine
{
	TriggerComponentBase::TriggerComponentBase(GameObject* _gameObject, const tstring& _typeName)
		: ComponentBase(_gameObject, _typeName)
	{

	}
	void TriggerComponentBase::OnStart()
	{

	}

	void TriggerComponentBase::OnChangeEnable(bool _newEnable)
	{

	}
}