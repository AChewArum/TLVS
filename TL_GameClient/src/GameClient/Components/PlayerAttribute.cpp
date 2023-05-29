#include "PlayerAttribute.h"

TL_GameEngine::GameObject* PlayerAttribute::Instance = nullptr;

void PlayerAttribute::OnStart()
{
	ComponentBase::OnStart();

	Instance = GetGameObject();
}
