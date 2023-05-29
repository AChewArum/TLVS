#pragma once
#include "GameEngine/GameApplication.h"
#include "GameEngine/Resource/Resource.h"

namespace TL_GameEngine
{
	class GameObject;
	class Scene;
}

class MoveTestApp :
    public TL_GameEngine::GameApplication
{
public:
	void OnApplicationStart() override;
	void OnApplicationTick() override;
	void OnApplicationEnd() override;

private:
	TL_GameEngine::Scene* m_Scene = nullptr;
	TL_GameEngine::GameObject* m_PlayerObj = nullptr;
	TL_GameEngine::GameObject* m_PlayerCameraObj = nullptr;
	TL_GameEngine::GameObject* m_HoverCameraObj = nullptr;


};

