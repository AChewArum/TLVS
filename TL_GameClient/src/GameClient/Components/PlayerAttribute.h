#pragma once
#include "GameEngine/GameFramework/Component/ComponentBase.h"

class PlayerAttribute :
	public TL_GameEngine::ComponentBase
{
public:
	static TL_GameEngine::GameObject* Instance;

	PlayerAttribute(TL_GameEngine::GameObject* _gameObject, const tstring& _typeName = TEXT("PlayerAttribute"))
		: ComponentBase(_gameObject, _typeName) {}

	void OnStart() override;
};
