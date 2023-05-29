#pragma once

#include "GameEngine/GameApplication.h"

namespace TL_GameEngine
{
	class Scene;
	class GameObject;
}

class TestGameApplication :
	public TL_GameEngine::GameApplication
{
public:
	void OnApplicationStart() override;

	void OnApplicationTick() override;

	void OnApplicationEnd() override;

private:
	TL_GameEngine::Scene* _scene;
	TL_GameEngine::Scene* _scene2;

	TL_GameEngine::GameObject* _gameObject1;
	TL_GameEngine::GameObject* _gameObject1_Child;
	TL_GameEngine::GameObject* _gameObject2;
	TL_GameEngine::GameObject* _gameObject2_Child;
};
