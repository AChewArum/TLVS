#pragma once

#include "GameEngine/GameApplication.h"
#include "GameEngine/Resource/Resource.h"

namespace TL_GameEngine
{
    class GameObject;
    class Scene;
}

class TestFirstStageGameApplication :
    public TL_GameEngine::GameApplication
{
public:
    void OnApplicationStart() override;

    void OnApplicationTick() override;

    void OnApplicationEnd() override;

private:
    TL_GameEngine::Scene* m_Scene;
    TL_GameEngine::GameObject* m_StageGO;
    TL_GameEngine::GameObject* m_Player;
    TL_GameEngine::GameObject* m_SoundSource1;
    TL_GameEngine::GameObject* m_SoundSource2;
    TL_GameEngine::GameObject* m_SoundSource3;
};
