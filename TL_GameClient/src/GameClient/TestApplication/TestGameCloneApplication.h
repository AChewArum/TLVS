#pragma once
#include "GameEngine/GameApplication.h"

namespace TL_GameEngine {
    class Scene;
    class GameObject;
}

class TestGameCloneApplication :
    public TL_GameEngine::GameApplication
{
public:
    void OnApplicationStart() override;

private:
    TL_GameEngine::Scene* _scene;

    TL_GameEngine::GameObject* _gameObject1;

    TL_GameEngine::GameObject* _gameObject2;
    TL_GameEngine::GameObject* _gameObject2_Child;

    TL_GameEngine::GameObject* _gameObject3;
    TL_GameEngine::GameObject* _gameObject3_Child;

    TL_GameEngine::GameObject* _gameObject1_Cloned;
    TL_GameEngine::GameObject* _gameObject2_Cloned;
    TL_GameEngine::GameObject* _gameObject3_Cloned;
};
